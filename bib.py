import pandas as pd
import numpy as np
import re
import os
from sklearn.tree import DecisionTreeClassifier

class Data():

    def __init__(self, simplify_classes:bool=True):
        self.features_train = pd.DataFrame()
        self.costs_train = pd.DataFrame()
        self.classes_train = pd.DataFrame()
        self.features_valid = pd.DataFrame()
        self.costs_valid = pd.DataFrame()
        self.classes_valid = pd.DataFrame()
        self.simplify_classes = simplify_classes

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

        self.features_train = df_train.iloc[:,1:11]
        self.classes_train = df_train.iloc[:,11]
        self.costs_train = df_train.iloc[:,12:23]

        self.features_valid = df_valid.iloc[:,1:11]
        self.classes_valid = df_valid.iloc[:,11]
        self.costs_valid = df_valid.iloc[:,12:23]

        if self.simplify_classes:
            self.classes_train[self.classes_train!=0] = 1
            self.classes_valid[self.classes_valid!=0] = 1
            self.simplify_costs()

    def simplify_costs(self):

        costs0_train = self.costs_train.iloc[:,0].to_frame()
        costs1_train = self.costs_train.iloc[:,1:].copy()
        costs1_train[costs1_train<0] = np.nan
        costs1_train_aux = costs1_train.min(axis=1)

        costs0_valid = self.costs_valid.iloc[:,0].to_frame()
        costs1_valid = self.costs_valid.iloc[:,1:].copy()
        costs1_valid[costs1_valid<0] = np.nan
        costs1_valid_aux = costs1_valid.min(axis=1)

        self.costs_train = costs0_train.join(costs1_train_aux.to_frame())
        self.costs_valid = costs0_valid.join(costs1_valid_aux.to_frame())

class Classifier(Data):
    def __init__(self,max_depth:int=5, random_state:int=42,):
        Data.__init__(self)
        self.max_depth=max_depth
        self.random_state=random_state
        self.total_cost = 0
        self.acc = 0

    def fit_tree(self):
        print('Fitting the decision tree')
        clf = DecisionTreeClassifier(max_depth=self.max_depth,random_state=self.random_state)
        clf.fit(self.features_train,self.classes_train)
        y_pred = clf.predict(self.features_valid)
        self.acc = clf.score(self.features_valid,self.classes_valid)
        self.total_cost = self.calculate_cost_of_decisions(y_pred)

    def calculate_cost_of_decisions(self, y_pred):
        cost = 0
        for i in range(len(y_pred)):
            if(y_pred[i] == 0):
                cost = cost + self.costs_valid.iloc[i,0]
            else:
                cost = cost + self.costs_valid.iloc[i,1]
        return cost

    def calculate_minimal_cost(self):
        cost = 0
        for i in range(len(self.classes_valid)):
            if(self.classes_valid.iloc[i] == 0):
                cost = cost + self.costs_valid.iloc[i,0]
            else:
                cost = cost + self.costs_valid.iloc[i,1]
        return cost
