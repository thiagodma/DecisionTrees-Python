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

data_QP_22 = Data()
data_QP_22.load_data(train_QP_22,valid_QP_22,ftk=[1,3,5,6,7,8])
clf_QP_22 = Classifier(data_QP_22,max_depth=5)
clf_QP_22.fit_tree()
clf_QP_22.prune_duplicate_leaves(clf_QP_22.clf)
clf_QP_22.get_stats()
print('Acc: '+ str(clf_QP_22.acc))
print('Cost:' + str(clf_QP_22.total_cost))
print('Min Cost: ' + str(clf_QP_22.calculate_minimal_cost()))
print('Ratio: ' + str(clf_QP_22.total_cost/clf_QP_22.calculate_minimal_cost()))
et = ExportTree(clf_QP_22)
et.write_tree_cpp(0,22,3)
print('\n')

data_QP_27 = Data()
data_QP_27.load_data(train_QP_27,valid_QP_27,ftk=[1,3,5,6,7,8])
clf_QP_27 = Classifier(data_QP_27, max_depth=6)
clf_QP_27.fit_tree()
clf_QP_27.prune_duplicate_leaves(clf_QP_27.clf)
clf_QP_27.get_stats()
print('Acc: '+ str(clf_QP_27.acc))
print('Cost:' + str(clf_QP_27.total_cost))
print('Min Cost: ' + str(clf_QP_27.calculate_minimal_cost()))
print('Ratio: ' + str(clf_QP_27.total_cost/clf_QP_27.calculate_minimal_cost()))
et = ExportTree(clf_QP_27)
et.write_tree_cpp(0,27,3)
print('\n')

data_QP_32 = Data()
data_QP_32.load_data(train_QP_32,valid_QP_32,ftk=[1,3,5,6,7,8])
clf_QP_32 = Classifier(data_QP_32, max_depth=10)
clf_QP_32.fit_tree()
clf_QP_32.prune_duplicate_leaves(clf_QP_32.clf)
clf_QP_32.get_stats()
print('Acc: '+ str(clf_QP_32.acc))
print('Cost:' + str(clf_QP_32.total_cost))
print('Min Cost: ' + str(clf_QP_32.calculate_minimal_cost()))
print('Ratio: ' + str(clf_QP_32.total_cost/clf_QP_32.calculate_minimal_cost()))
et = ExportTree(clf_QP_32)
et.write_tree_cpp(0,32,3)
print('\n')

data_QP_37 = Data()
data_QP_37.load_data(train_QP_37,valid_QP_37,ftk=[1,3,5,6,7,8])
clf_QP_37 = Classifier(data_QP_37, max_depth=7)
clf_QP_37.fit_tree()
clf_QP_37.prune_duplicate_leaves(clf_QP_37.clf)
clf_QP_37.get_stats()
print('Acc: '+ str(clf_QP_37.acc))
print('Cost:' + str(clf_QP_37.total_cost))
print('Min Cost: ' + str(clf_QP_37.calculate_minimal_cost()))
print('Ratio: ' + str(clf_QP_37.total_cost/clf_QP_37.calculate_minimal_cost()))
et = ExportTree(clf_QP_37)
et.write_tree_cpp(0,37,3)
print('\n')
