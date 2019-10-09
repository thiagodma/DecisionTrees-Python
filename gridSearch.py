from bib import *
from sklearn.tree.export import export_text
import matplotlib.pyplot as plt

train_QP_22 = ['ChristmasTree_QP_22_depth1.csv','CrowdRun_QP_22_depth1.csv','DucksTakeOff_QP_22_depth1.csv',
'PedestrianArea_QP_22_depth1.csv','RushHour_QP_22_depth1.csv','Sunflower_QP_22_depth1.csv']
valid_QP_22 = ['Tractor_QP_22_depth1.csv','Wisley_QP_22_depth1.csv']

data_QP_22 = Data()
data_QP_22.load_data(train_QP_22,valid_QP_22)


max_depths = [2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,24,26,29,33,38,44,47,50,55]

costs =[]

for max_depth in max_depths:
    clf_QP_22 = Classifier(data_QP_22,max_depth=max_depth)
    clf_QP_22.fit_tree()
    clf_QP_22.hack_tree()
    clf_QP_22.get_stats()
    print('Acc: '+ str(clf_QP_22.acc))
    print('Cost:' + str(clf_QP_22.total_cost))
    print('Min Cost: ' + str(clf_QP_22.calculate_minimal_cost()))
    print('Ratio: ' + str(clf_QP_22.total_cost/clf_QP_22.calculate_minimal_cost()))
    costs.append(clf_QP_22.total_cost)
    print('\n')

plt.plot(max_depths,costs)
plt.show()
