from bib import *
import matplotlib.pyplot as plt
from sklearn.model_selection import GridSearchCV
from sklearn.pipeline import Pipeline
import pickle

#Later I'll do a 10-fold cross validation so I don't need a explicit validation set
train_QP_22 = ['ChristmasTree_QP_22_depth1.csv','CrowdRun_QP_22_depth1.csv','DucksTakeOff_QP_22_depth1.csv',
'PedestrianArea_QP_22_depth1.csv','RushHour_QP_22_depth1.csv','Sunflower_QP_22_depth1.csv','Tractor_QP_22_depth1.csv','Wisley_QP_22_depth1.csv']
train_QP_27 = ['ChristmasTree_QP_27_depth1.csv','CrowdRun_QP_27_depth1.csv','DucksTakeOff_QP_27_depth1.csv',
'PedestrianArea_QP_27_depth1.csv','RushHour_QP_27_depth1.csv','Sunflower_QP_27_depth1.csv','Tractor_QP_27_depth1.csv','Wisley_QP_27_depth1.csv']
train_QP_32 = ['ChristmasTree_QP_32_depth1.csv','CrowdRun_QP_32_depth1.csv','DucksTakeOff_QP_32_depth1.csv',
'PedestrianArea_QP_32_depth1.csv','RushHour_QP_32_depth1.csv','Sunflower_QP_32_depth1.csv','Tractor_QP_32_depth1.csv','Wisley_QP_32_depth1.csv']
train_QP_37 = ['ChristmasTree_QP_37_depth1.csv','CrowdRun_QP_37_depth1.csv','DucksTakeOff_QP_37_depth1.csv',
'PedestrianArea_QP_37_depth1.csv','RushHour_QP_37_depth1.csv','Sunflower_QP_37_depth1.csv','Tractor_QP_37_depth1.csv','Wisley_QP_37_depth1.csv']
train_d1 = [train_QP_22,train_QP_27,train_QP_32,train_QP_37]


res = []
for item in train_d1:

    data = Data()
    #I'm lazy...
    data.load_data(item,item)

    #matrix with all the features
    X = data.features_train
    #vector with all the classes
    y = data.classes_train
    #matrix with all the costs
    C = data.costs_train
    #import pdb; pdb.set_trace()
    pipe = Pipeline([('classifier' , DecisionTreeClassifier(random_state=42))])

    #here I choose what paramaters I want to to the grid search on
    param_grid = [
        {'classifier' : [DecisionTreeClassifier()],
        'classifier__criterion' : ['gini','entropy'],
        'classifier__splitter'  : ['best','random'],
        'classifier__min_samples_leaf' : np.logspace(0,4,num=10,dtype=np.int64)
        }]

    # Create grid search object
    grid = GridSearchCV(pipe, param_grid = param_grid, cv = 30, verbose=True, n_jobs=-1)
    grid.fit(X,y)
    df = pd.DataFrame(grid.cv_results_)
    #gets info about the best classifier
    output = df.iloc[np.argmax(df['mean_test_score']),:]

    res.append(output)

#saves the best parameters for each classifier
with open("GridSearch_results_d1.pkl", "wb") as fp: pickle.dump(res, fp)
