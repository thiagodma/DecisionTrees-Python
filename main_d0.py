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


data_QP_22 = Data()
data_QP_22.load_data(train_QP_22,valid_QP_22)
clf_QP_22 = Classifier(data_QP_22,max_depth=6,qp_as_feature=False)
clf_QP_22.fit_tree()
print('Acc: '+ str(clf_QP_22.acc))
print('Cost:' + str(clf_QP_22.total_cost))
print('Min Cost: ' + str(clf_QP_22.calculate_minimal_cost()))
print('Ratio: ' + str(clf_QP_22.total_cost/clf_QP_22.calculate_minimal_cost()))
print('\n')

data_QP_27 = Data()
data_QP_27.load_data(train_QP_27,valid_QP_27)
clf_QP_27 = Classifier(data_QP_27,max_depth=4)
clf_QP_27.fit_tree()
print('Acc: '+ str(clf_QP_27.acc))
print('Cost:' + str(clf_QP_27.total_cost))
print('Min Cost: ' + str(clf_QP_27.calculate_minimal_cost()))
print('Ratio: ' + str(clf_QP_27.total_cost/clf_QP_27.calculate_minimal_cost()))
print('\n')

data_QP_32 = Data()
data_QP_32.load_data(train_QP_32,valid_QP_32)
clf_QP_32 = Classifier(data_QP_32,max_depth=3)
clf_QP_32.fit_tree()
print('Acc: '+ str(clf_QP_32.acc))
print('Cost:' + str(clf_QP_32.total_cost))
print('Min Cost: ' + str(clf_QP_32.calculate_minimal_cost()))
print('Ratio: ' + str(clf_QP_32.total_cost/clf_QP_32.calculate_minimal_cost()))
print('\n')

data_QP_37 = Data()
data_QP_37.load_data(train_QP_37,valid_QP_37)
clf_QP_37 = Classifier(data_QP_37,max_depth=8)
clf_QP_37.fit_tree()
print('Acc: '+ str(clf_QP_37.acc))
print('Cost:' + str(clf_QP_37.total_cost))
print('Min Cost: ' + str(clf_QP_37.calculate_minimal_cost()))
print('Ratio: ' + str(clf_QP_37.total_cost/clf_QP_37.calculate_minimal_cost()))
print('\n')
