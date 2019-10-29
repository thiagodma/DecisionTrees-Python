from bib import *
import matplotlib.pyplot as plt

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
depths = np.linspace(1,10,num=10,dtype=np.int64)
#FT_QPs = [[23,24,25],[28,29,30],[33,34,35],[38,39,40]]

#one parameter per QP
best_parameters = []

for train,valid in zip(trains,valids):
    data = Data()
    data.load_data(train,valid,ftk=[5,6,8])

    accs = []
    costs = []
    for depth in depths:

        clf = Classifier(data,max_depth=depth,class_weight=None)
        clf.fit_tree()
        clf.prune_duplicate_leaves(clf.clf)
        #clf.hack_tree()
        clf.get_stats()
        #print('Acc: '+ str(clf_QP_32.acc))
        #print('Cost:' + str(clf_QP_32.total_cost))
        #print('Min Cost: ' + str(clf_QP_32.calculate_minimal_cost()))
        #print('Ratio: ' + str(clf_QP_32.total_cost/clf_QP_32.calculate_minimal_cost()))
        accs.append(clf.acc)
        costs.append(clf.total_cost)

    best_parameters.append((depths[np.argmax(accs)],costs[np.argmax(accs)]))

for best_parameter in best_parameters: print(best_parameter)

        #print('Min cost parameter: ' + str(min_samples_leafs[np.argmin(costs)]))
        #print('Max accuracy parameter: ' + str(min_samples_leafs[np.argmax(accs)]))
        #print('Min cost: ' + str(np.min(costs)))
        #print('Max accuracy: ' + str(np.max(accs)))
        #print('Cost from max acc: ' + str(costs[np.argmax(accs)]))

        #plt.subplot(2, 1, 1)
        #plt.plot(min_samples_leafs, accs, '-', lw=2)
        #plt.xlabel('min_samples_leaf')
        #plt.ylabel('Accuracy')
        #plt.subplot(2,1,2)
        #plt.plot(min_samples_leafs, costs, '-', lw=2)
        #plt.xlabel('min_samples_leaf')
        #plt.ylabel('Cost')
        #plt.tight_layout()
        #plt.show()
