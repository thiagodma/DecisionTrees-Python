from bib import *
import matplotlib.pyplot as plt

train_QP_22 = ['BasketballDrive_QP_22_depth0.csv']
train_QP_27 = ['BasketballDrive_QP_27_depth0.csv']
train_QP_32 = ['BasketballDrive_QP_32_depth0.csv']
train_QP_37 = ['BasketballDrive_QP_37_depth0.csv']

# train_QP_22 = ['BQTerrace_QP_22_depth0.csv']
# train_QP_27 = ['BQTerrace_QP_27_depth0.csv']
# train_QP_32 = ['BQTerrace_QP_32_depth0.csv']
# train_QP_37 = ['BQTerrace_QP_37_depth0.csv']
#
# train_QP_22 = ['Cactus_QP_22_depth0.csv']
# train_QP_27 = ['Cactus_QP_27_depth0.csv']
# train_QP_32 = ['Cactus_QP_32_depth0.csv']
# train_QP_37 = ['Cactus_QP_37_depth0.csv']
#
# train_QP_22 = ['Kimono_QP_22_depth0.csv']
# train_QP_27 = ['Kimono_QP_27_depth0.csv']
# train_QP_32 = ['Kimono_QP_32_depth0.csv']
# train_QP_37 = ['Kimono_QP_37_depth0.csv']
#
# train_QP_22 = ['ParkScene_QP_22_depth0.csv']
# train_QP_27 = ['ParkScene_QP_27_depth0.csv']
# train_QP_32 = ['ParkScene_QP_32_depth0.csv']
# train_QP_37 = ['ParkScene_QP_37_depth0.csv']


trains = [train_QP_22, train_QP_27, train_QP_32, train_QP_37]
valids = [valid_QP_22, valid_QP_27, valid_QP_32, valid_QP_37]
depths = np.linspace(1,10,num=5,dtype=np.int64)
#FT_QPs = [[23,24,25],[28,29,30],[33,34,35],[38,39,40]]

#one parameter per QP
best_parameters = []

qps = [22,27,32,37]
for train,valid,qp in zip(trains,valids,qps):
    data = Data()
    #if qp == 27 or qp==37:
    #    data.load_data(train,valid,ftk=[5,6,8])
    #else:
    data.load_data(train,valid)
    accs = []
    costs = []
    for depth in depths:

        #if qp == 22:
        clf = Classifier(data,max_depth=depth)
        #else:
        #    clf = Classifier(data,max_depth=depth,hack=True)
        #clf.fit_tree()
        clf.fit_tree()
        clf.prune_duplicate_leaves(clf.clf)
        clf.get_stats()
        accs.append(clf.acc)
        costs.append(clf.total_cost)

    best_parameters.append((depths[np.argmin(costs)],costs[np.argmin(costs)]))

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
