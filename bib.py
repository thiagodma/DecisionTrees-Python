import pandas as pd
import numpy as np
import re
import os
from sklearn.tree import DecisionTreeClassifier
from sklearn.tree.export import export_text

class Data():

    def __init__(self):
        self.features_train = []
        self.costs_train = []
        self.classes_train = []
        self.features_valid = []
        self.costs_valid = []
        self.classes_valid = []

    def get_data(self):
        '''
        Imports training data into a folder named 'Data'.
        '''
        if not os.path.isdir('Data'): os.mkdir('Data')

        d0_files = ['Features_Depth_0_Training_1.txt','Features_Depth_0_Training_2.txt',
        'Features_Depth_0_Training_3.txt','Features_Depth_0_Training_4.txt','Features_Depth_0_Training_5.txt']

        d1_files = ['Features_Depth_1_Training_1.txt', 'Features_Depth_1_Training_2.txt',
        'Features_Depth_1_Training_3.txt','Features_Depth_1_Training_4.txt','Features_Depth_1_Training_5.txt']

        seqs = list(next(os.walk('FeaturesDataset'))[1])
        for seq in seqs:
            qps = list(next(os.walk('FeaturesDataset/' + seq))[1])
            for qp in qps:
                df0 = pd.DataFrame()
                df1 = pd.DataFrame()
                for d0_file in d0_files:
                    aux = pd.read_csv('FeaturesDataset/' + seq + '/' + qp + '/HEVC/' + d0_file, sep=' ', names=list(range(25)), header=None)
                    aux = aux.drop(24,axis=1)
                    df0 = pd.concat([df0,aux], sort=False, )
                for d1_file in d1_files:
                    aux = pd.read_csv('FeaturesDataset/' + seq + '/' + qp + '/HEVC/' + d1_file, sep=' ', names=list(range(25)), header=None)
                    aux = aux.drop(24,axis=1)
                    df1 = pd.concat([df1,aux],sort=False)
                df0.to_csv('Data/'+seq+'_'+qp+'_depth0.csv',sep='|',encoding='utf-8',index=False, )
                df1.to_csv('Data/'+seq+'_'+qp+'_depth1.csv',sep='|',encoding='utf-8',index=False)

    def load_data(self, training_seqs, valid_seqs):
        '''
        Loads the data in the class
        '''

        print('Loading the data...')

        df_train = pd.DataFrame()
        for training_seq in training_seqs:
            aux = pd.read_csv('Data/'+training_seq,sep='|')
            df_train = pd.concat([df_train,aux], sort=False)

        df_valid = pd.DataFrame()
        for valid_seq in valid_seqs:
            aux = pd.read_csv('Data/'+valid_seq,sep='|')
            df_valid = pd.concat([df_valid,aux], sort=False)

        f = df_train.iloc[:,1:11].values
        qp = df_train.iloc[:,23].values
        qp.shape = (qp.shape[0],1)
        self.features_train = np.hstack((f,qp))
        self.classes_train = df_train.iloc[:,11].values
        self.costs_train = df_train.iloc[:,12:23].values

        f = df_valid.iloc[:,1:11].values
        qp = df_valid.iloc[:,23].values
        qp.shape = (qp.shape[0],1)
        self.features_valid = np.hstack((f,qp))
        self.classes_valid = df_valid.iloc[:,11].values
        self.costs_valid = df_valid.iloc[:,12:23].values

        self.classes_train[self.classes_train!=0] = 1
        self.classes_valid[self.classes_valid!=0] = 1

        self.simplify_costs()
        self.correct_misclassified_samples()

    def simplify_costs(self):

        costs0_train = self.costs_train[:,0]
        costs1_train = self.costs_train[:,1:]
        costs1_train[costs1_train<0] = np.inf
        costs1_train_aux = np.amin(costs1_train,axis=1)

        costs0_valid = self.costs_valid[:,0]
        costs1_valid = self.costs_valid[:,1:]
        costs1_valid[costs1_valid<0] = np.inf
        costs1_valid_aux = np.amin(costs1_valid,axis=1)

        self.costs_train = np.column_stack((costs0_train,costs1_train_aux))
        self.costs_valid = np.column_stack((costs0_valid,costs1_valid_aux))

    def correct_misclassified_samples(self):

        for i in range(self.costs_train.shape[0]):
            if self.costs_train[i][0] <= self.costs_train[i][1]:
                self.classes_train[i] = 0
            else:
                self.classes_train[i] = 1

        for i in range(self.costs_valid.shape[0]):
            if self.costs_valid[i][0] <= self.costs_valid[i][1]:
                self.classes_valid[i] = 0
            else:
                self.classes_valid[i] = 1


class Classifier():
    def __init__(self,data:Data, max_depth:int=5, random_state:int=42, splitter:str='best',
    min_samples_split:int=2,min_samples_leaf:int=1 ,qp_as_feature:bool=False):
        self.max_depth=max_depth
        self.random_state=random_state
        self.total_cost = 0
        self.acc = 0
        self.data = data
        self.clf = None
        self.splitter=splitter
        self.min_samples_leaf = min_samples_leaf
        self.min_samples_split = min_samples_split
        self.qp_as_feature = qp_as_feature

    def fit_tree(self):
        print('Fitting the decision tree')
        clf = DecisionTreeClassifier(max_depth=self.max_depth,random_state=self.random_state,splitter=self.splitter,
        min_samples_split=self.min_samples_split, min_samples_leaf=self.min_samples_leaf)

        if(self.qp_as_feature):
            clf.fit(self.data.features_train,self.data.classes_train)
        else:
            clf.fit(self.data.features_train[:,:-1],self.data.classes_train)
        self.clf = clf

    def get_stats(self):
        clf=self.clf
        y_pred = clf.predict(self.data.features_valid[:,:-1])
        self.acc = clf.score(self.data.features_valid[:,:-1],self.data.classes_valid)
        self.total_cost = self.calculate_cost_of_decisions(y_pred)

    def calculate_cost_of_decisions(self, y_pred):
        cost = 0
        for i in range(len(y_pred)):
            if(y_pred[i] == 0):
                cost = cost + self.data.costs_valid[i,0]
            else:
                #if 'not split' is not available
                if self.data.costs_valid[i,1] == np.inf:
                    cost = cost + self.data.costs_valid[i,0]
                else:
                    cost = cost + self.data.costs_valid[i,1]
        return cost

    def calculate_minimal_cost(self):
        cost = 0
        for i in range(len(self.data.classes_valid)):
            if(self.data.classes_valid[i] == 0):
                cost = cost + self.data.costs_valid[i,0]
            else:
                cost = cost + self.data.costs_valid[i,1]
        return cost
