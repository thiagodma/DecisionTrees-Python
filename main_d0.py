from bib import *

train_QP_22 = ['ChristmasTree_QP_22_depth0.csv','CrowdRun_QP_22_depth0.csv','DucksTakeOff_QP_22_depth0.csv',
'PedestrianArea_QP_22_depth0.csv','RushHour_QP_22_depth0.csv','Sunflower_QP_22_depth0.csv']
valid_QP_22 = ['Tractor_QP_22_depth0.csv','Wisley_QP_22_depth0.csv']

train_QP_27 = ['ChristmasTree_QP_27_depth0.csv','CrowdRun_QP_27_depth0.csv','DucksTakeOff_QP_27_depth0.csv',
'PedestrianArea_QP_27_depth0.csv','RushHour_QP_27_depth0.csv','Sunflower_QP_27_depth0.csv']
valid_QP_27 = ['Tractor_QP_27_depth0.csv','Wisley_QP_27_depth0.csv']

train_QP_32 = ['ChristmasTree_QP_32_depth0.csv','CrowdRun_QP_32_depth0.csv','DucksTakeOff_QP_32_depth0.csv',
'PedestrianArea_QP_32_depth0.csv','RushHour_QP_32_depth0.csv','Sunflower_QP_32_depth0.csv']
valid_QP_32 = ['Tractor_QP_32_depth0.csv','Wisley_QP_32_depth0.csv']

train_QP_37 = ['ChristmasTree_QP_37_depth0.csv','CrowdRun_QP_37_depth0.csv','DucksTakeOff_QP_37_depth0.csv',
'PedestrianArea_QP_37_depth0.csv','RushHour_QP_37_depth0.csv','Sunflower_QP_37_depth0.csv']
valid_QP_37 = ['Tractor_QP_37_depth0.csv','Wisley_QP_37_depth0.csv']

#checks if there is a 'tree.cpp' file. If so, deletes it
if os.path.isfile('tree.cpp'): os.remove('tree.cpp')

max_depths = [7,3,3,7]
trains = [train_QP_22, train_QP_27, train_QP_32, train_QP_37]
valids = [valid_QP_22, valid_QP_27, valid_QP_32, valid_QP_37]
qps = [22,27,32,37]

for train,valid,max_depth,qp in zip(trains,valids,max_depths,qps):

    data = Data()
    data.load_data(train,valid,)
    clf = Classifier(data,max_depth=max_depth)
    clf.fit_tree()
    clf.prune_duplicate_leaves(clf.clf)
    clf.get_stats()
    print('Acc: '+ str(clf.acc))
    print('Cost:' + str(clf.total_cost))
    print('Min Cost: ' + str(clf.calculate_minimal_cost()))
    print('Ratio: ' + str(clf.total_cost/clf.calculate_minimal_cost()))
    et = ExportTree(clf)
    et.write_tree_cpp(0,qp,3)
    print('\n')
