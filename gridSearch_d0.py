from bib import *
import matplotlib.pyplot as plt
from sklearn.model_selection import GridSearchCV
from sklearn.pipeline import Pipeline
import pickle
from sklearn.model_selection import PredefinedSplit

#Later I'll do a 10-fold cross validation so I don't need a explicit validation set
train_QP_22 = ['ChristmasTree_QP_22_depth0.csv','CrowdRun_QP_22_depth0.csv','DucksTakeOff_QP_22_depth0.csv',
'PedestrianArea_QP_22_depth0.csv','RushHour_QP_22_depth0.csv','Sunflower_QP_22_depth0.csv','Tractor_QP_22_depth0.csv','Wisley_QP_22_depth0.csv']
train_QP_27 = ['ChristmasTree_QP_27_depth0.csv','CrowdRun_QP_27_depth0.csv','DucksTakeOff_QP_27_depth0.csv',
'PedestrianArea_QP_27_depth0.csv','RushHour_QP_27_depth0.csv','Sunflower_QP_27_depth0.csv','Tractor_QP_27_depth0.csv','Wisley_QP_27_depth0.csv']
train_QP_32 = ['ChristmasTree_QP_32_depth0.csv','CrowdRun_QP_32_depth0.csv','DucksTakeOff_QP_32_depth0.csv',
'PedestrianArea_QP_32_depth0.csv','RushHour_QP_32_depth0.csv','Sunflower_QP_32_depth0.csv','Tractor_QP_32_depth0.csv','Wisley_QP_32_depth0.csv']
train_QP_37 = ['ChristmasTree_QP_37_depth0.csv','CrowdRun_QP_37_depth0.csv','DucksTakeOff_QP_37_depth0.csv',
'PedestrianArea_QP_37_depth0.csv','RushHour_QP_37_depth0.csv','Sunflower_QP_37_depth0.csv','Tractor_QP_37_depth0.csv','Wisley_QP_37_depth0.csv']
train_d0 = [train_QP_22,train_QP_27,train_QP_32,train_QP_37]

accs = []
clfs = []
for item in train_d0:

    accs_aux = []
    clfs_aux = []
    accs_mat = np.zeros((8,40))
    for i in range(8):

        valid = [item[i]]
        train = [item[j] for j in range(8) if j!=i]
        #import pdb; pdb.set_trace()
        data = Data()
        #I'm lazy...
        data.load_data(train,valid)

        #matrix with all the features in the training set
        X_train = data.features_train[:,:10]
        #matrix with all the features in the valid set
        X_valid = data.features_valid[:,:10]
        #vector with all the classes in the training set
        y_train = data.classes_train
        y_train.shape = (y_train.shape[0],1)
        #vector with all the classes in the valid set
        y_valid = data.classes_valid
        y_valid.shape = (y_valid.shape[0],1)

        test_fold = np.zeros(X_train.shape[0]+X_valid.shape[0])
        test_fold[X_train.shape[0]:] = test_fold[X_train.shape[0]:] -1

        ps = PredefinedSplit(test_fold=test_fold)

        X = np.vstack((X_train,X_valid))
        y = np.vstack((y_train,y_valid))

        #matrix with all the costs
        #C = data.costs_train
        #import pdb; pdb.set_trace()
        pipe = Pipeline([('classifier' , DecisionTreeClassifier())])

        #here I choose what paramaters I want to to the grid search on
        param_grid = [
            {'classifier' : [DecisionTreeClassifier()],
            'classifier__random_state' : [42],
            'classifier__criterion' : ['gini','entropy'],
            'classifier__splitter' : ['best','random'],
            'classifier__min_samples_leaf' : np.logspace(0,4,num=10,dtype=np.int64)
            }]

        # Create grid search object
        grid = GridSearchCV(pipe, param_grid = param_grid, cv = ps, verbose=True, n_jobs=-1)
        grid.fit(X,y)
        df = pd.DataFrame(grid.cv_results_)

        accs_mat[i,:] = df.split0_test_score.values
        clfs_qp = df.param_classifier

        #import pdb; pdb.set_trace()
        #gets info about the best classifier
        #output = df.iloc[np.argmax(np.array(df.mean_test_score.values)),:]
        #import pdb; pdb.set_trace()
        #accs_aux.append(output.mean_test_score)
        #clfs_aux.append(output.param_classifier)

    accs.append(accs_mat)
    clfs.append(clfs_qp)


#saves the best parameters for each classifier
res = [accs,clfs]
with open("GridSearch_results_d0.pkl", "wb") as fp: pickle.dump(res, fp)

best_clfs = []

for acc_qp, clf_qp in zip(accs,clfs):
    accs_mean = np.mean(acc_qp,axis=0)
    accs_std = np.std(acc_qp,axis=0)
    scores = accs_mean/accs_std
    idx = np.argmax(scores)
    best_clfs.append(clf_qp[idx])

with open("best_clfs_d0.pkl", "wb") as fp: pickle.dump(best_clfs,fp)
