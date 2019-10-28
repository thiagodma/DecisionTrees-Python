import pandas as pd
import numpy as np
import re
import os
from sklearn.tree import DecisionTreeClassifier
from sklearn.tree.export import export_text
from sklearn.tree._tree import TREE_LEAF

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

    def load_data(self, training_seqs:list, valid_seqs:list, balanced:bool=True):
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

        if balanced: self.balance_data()

    def balance_data(self):
        '''
        n1 = np.sum(self.classes_train)
        n0 = len(self.classes_train) - n1

        if n0 > n1:
            idxs0 = np.where(self.classes_train == 0)[0]
            idxs1 = np.where(self.classes_train == 1)[0]
            #import pdb; pdb.set_trace()
            idxs0_keep = np.random.choice(idxs0,size=n1.astype('int'),replace=False)
            self.features_train = np.vstack((self.features_train[idxs0_keep,:],self.features_train[idxs1,:]))
            self.classes_train = np.hstack((self.classes_train[idxs0_keep],self.classes_train[idxs1]))
        if n1 > n0:
            idxs0 = np.where(self.classes_train == 0)[0]
            idxs1 = np.where(self.classes_train == 1)[0]
            idxs1_keep = np.random.choice(idxs1,size=n0.astype('int'),replace=False)
            self.features_train = np.vstack((self.features_train[idxs0,:],self.features_train[idxs1_keep,:]))
            self.classes_train = np.hstack((self.classes_train[idxs0],self.classes_train[idxs1_keep]))
        '''

        n1 = np.sum(self.classes_valid)
        n0 = len(self.classes_valid) - n1

        if n0 > n1:
            idxs0 = np.where(self.classes_valid == 0)[0]
            idxs1 = np.where(self.classes_valid == 1)[0]
            #import pdb; pdb.set_trace()
            idxs0_keep = np.random.choice(idxs0,size=n1.astype('int'),replace=False)
            self.features_valid = np.vstack((self.features_valid[idxs0_keep,:],self.features_valid[idxs1,:]))
            self.classes_valid = np.hstack((self.classes_valid[idxs0_keep],self.classes_valid[idxs1]))
            self.costs_valid = np.vstack((self.costs_valid[idxs0_keep,:],self.costs_valid[idxs1,:]))
            #import pdb; pdb.set_trace()
        if n1 > n0:
            idxs0 = np.where(self.classes_valid == 0)[0]
            idxs1 = np.where(self.classes_valid == 1)[0]
            idxs1_keep = np.random.choice(idxs1,size=n0.astype('int'),replace=False)
            self.features_valid = np.vstack((self.features_valid[idxs0,:],self.features_valid[idxs1_keep,:]))
            self.classes_valid = np.hstack((self.classes_valid[idxs0],self.classes_valid[idxs1_keep]))
            self.costs_valid = np.vstack((self.costs_valid[idxs0,:],self.costs_valid[idxs1_keep,:]))
            #import pdb; pdb.set_trace()

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
    min_samples_split:int=2,min_samples_leaf:int=1 ,qp_as_feature:bool=False, criterion:str='gini',class_weight=None):
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
        self.criterion = criterion
        self.class_weight = class_weight

    def fit_tree(self):
        print('Fitting the decision tree')
        clf = DecisionTreeClassifier(max_depth=self.max_depth,random_state=self.random_state,splitter=self.splitter,
        min_samples_split=self.min_samples_split, min_samples_leaf=self.min_samples_leaf,criterion=self.criterion,class_weight = self.class_weight)

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

    def is_leaf(self, inner_tree, index):
        # Check whether node is leaf node
        return (inner_tree.children_left[index] == TREE_LEAF and
                inner_tree.children_right[index] == TREE_LEAF)

    def prune_index(self, inner_tree, decisions, index=0):
        # Start pruning from the bottom - if we start from the top, we might miss
        # nodes that become leaves during pruning.
        # Do not use this directly - use prune_duplicate_leaves instead.
        if not self.is_leaf(inner_tree, inner_tree.children_left[index]):
            self.prune_index(inner_tree, decisions, inner_tree.children_left[index])
        if not self.is_leaf(inner_tree, inner_tree.children_right[index]):
            self.prune_index(inner_tree, decisions, inner_tree.children_right[index])

        # Prune children if both children are leaves now and make the same decision:
        if (self.is_leaf(inner_tree, inner_tree.children_left[index]) and
            self.is_leaf(inner_tree, inner_tree.children_right[index]) and
            (decisions[index] == decisions[inner_tree.children_left[index]]) and
            (decisions[index] == decisions[inner_tree.children_right[index]])):
            # turn node into a leaf by "unlinking" its children
            inner_tree.children_left[index] = TREE_LEAF
            inner_tree.children_right[index] = TREE_LEAF
            ##print("Pruned {}".format(index))

    def prune_duplicate_leaves(self, mdl):
        # Remove leaves if both
        decisions = mdl.tree_.value.argmax(axis=2).flatten().tolist() # Decision for each node
        self.prune_index(mdl.tree_, decisions)

class ExportTree():
    def __init__(self,classifier:Classifier):
        self.clf = classifier.clf

    def _get_ntabs(self,line):
        return len(re.findall(r'\|   ',line))

    def _isIf(self,line):
        return len(re.findall(r'<=',line)) != 0

    def _isLeaf(self,line):
        return len(re.findall('class',line)) != 0

    def _getIf(self,line):
        n_tabs = self._get_ntabs(line)
        feature = re.findall(r'_\d',line)[0][1]
        val = re.findall(r'\d+.\d+',line)[0]
        return '    '*n_tabs + 'if(dFeatures[' + feature + '] <= ' + val + ')\n'

    def _getClass(self,line):
        return re.findall(r'class: (\d).\d',line)[0]

    def _getLeaf(self,line):
        return '    '*self._get_ntabs(line) + 'return ' + self._getClass(line) + ';\n'

    def _get_Key(self,line,n_tabs_history):
        n_tabs = n_tabs_history.pop(-1)
        n_tabs = self._get_ntabs(line)

        #i have to open keys
        if n_tabs not in n_tabs_history:
            key = '    '*n_tabs + '{\n'
            n_tabs_history.append(n_tabs)
            return key,n_tabs_history
        #i have to close keys
        else:
            key = '    '*n_tabs + '}\n'
            n_tabs_history.remove(n_tabs)
            return key, n_tabs_history

    def write_tree_cpp(self, depth:int, qp:int,version:int):

        lines = export_text(self.clf,max_depth=1000)
        lines = lines.split('\n')
        lines = lines[:-1]

        fo = open('tree.cpp','a+')
        n_tabs_history = []

        fo.write('UInt TTrEngine::xdecide_depth'+str(depth)+'_QP'+str(qp)+'_v'+str(version)+'(Double *dFeatures)\n{\n')
        for line in lines:
            n_tabs_history.append(self._get_ntabs(line))
            if self._isIf(line):
                #writes the 'if' statement
                fo.write(self._getIf(line))
                #writes the key (opens it or closes it)
                #key, n_tabs_history = self._get_Key(line,n_tabs_history)
                key = '    '*self._get_ntabs(line) + '{\n'
                #if len(re.findall(r'}',key)) > 0:
                #    import pdb; pdb.set_trace()
                fo.write(key)
            elif self._isLeaf(line):
                #writes the leaf
                fo.write(self._getLeaf(line))
            else:

                count = n_tabs_history[-2] - n_tabs_history[-1]
                while(count>1):
                    fo.write('    '*(n_tabs_history[-1]+count-1) + '}\n')
                    count-=1

                #writes the key (opens it or closes it)
                #key, n_tabs_history = self._get_Key(line,n_tabs_history)
                key = '    '*self._get_ntabs(line) + '}\n'
                fo.write(key)
                #writes the else statement
                fo.write('    '*self._get_ntabs(line) + 'else\n')
                fo.write('    '*self._get_ntabs(line) + '{\n')

        count = self._get_ntabs(lines[-1])
        while(count>0):
            fo.write('    '*count + '}\n')
            count-=1

        fo.write('}\n\n')
        fo.close()
