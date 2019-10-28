from bib import *

train_QP_22 = ['ChristmasTree_QP_22_depth1.csv','CrowdRun_QP_22_depth1.csv','DucksTakeOff_QP_22_depth1.csv',
'PedestrianArea_QP_22_depth1.csv','RushHour_QP_22_depth1.csv','Sunflower_QP_22_depth1.csv']
valid_QP_22 = ['Tractor_QP_22_depth1.csv','Wisley_QP_22_depth1.csv']

train_QP_27 = ['ChristmasTree_QP_27_depth1.csv','CrowdRun_QP_27_depth1.csv','DucksTakeOff_QP_27_depth1.csv',
'PedestrianArea_QP_27_depth1.csv','RushHour_QP_27_depth1.csv','Sunflower_QP_27_depth1.csv']
valid_QP_27 = ['Tractor_QP_27_depth1.csv','Wisley_QP_27_depth1.csv']

train_QP_32 = ['ChristmasTree_QP_32_depth1.csv','CrowdRun_QP_32_depth1.csv','DucksTakeOff_QP_32_depth1.csv',
'PedestrianArea_QP_32_depth1.csv','RushHour_QP_32_depth1.csv','Sunflower_QP_32_depth1.csv']
valid_QP_32 = ['Tractor_QP_32_depth1.csv','Wisley_QP_32_depth1.csv']

train_QP_37 = ['ChristmasTree_QP_37_depth1.csv','CrowdRun_QP_37_depth1.csv','DucksTakeOff_QP_37_depth1.csv',
'PedestrianArea_QP_37_depth1.csv','RushHour_QP_37_depth1.csv','Sunflower_QP_37_depth1.csv']
valid_QP_37 = ['Tractor_QP_37_depth1.csv','Wisley_QP_37_depth1.csv']

trains = [train_QP_22, train_QP_27, train_QP_32, train_QP_37]
valids = [valid_QP_22, valid_QP_27, valid_QP_32, valid_QP_37]
ft_qps_all = [[23,24,25],[28,29,30],[33,34,35],[38,39,40]]
max_depths_all = [[8,2,6],[10,8,6],[8,6,8],[8,2,6]]

for train,valid,ft_qps,max_depths in zip(trains,valids,ft_qps_all,max_depths_all):
    for ft_qp,max_depth in zip(ft_qps,max_depths):
        data = Data()
        data.load_data(train,valid,ft_qp=ft_qp)
        clf = Classifier(data,max_depth=max_depth)
        clf.fit_tree()
        clf.prune_duplicate_leaves(clf.clf)
        clf.get_stats()
        print('Acc: '+ str(clf.acc))
        print('Cost:' + str(clf.total_cost))
        print('Min Cost: ' + str(clf.calculate_minimal_cost()))
        print('Ratio: ' + str(clf.total_cost/clf.calculate_minimal_cost()))
        et = ExportTree(clf)
        et.write_tree_cpp(1,ft_qp,3)
        print('\n')
