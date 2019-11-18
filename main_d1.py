from bib import *

train_QP_22 = ['Cactus_QP_22_depth1.csv']
train_QP_27 = ['Cactus_QP_27_depth1.csv']
train_QP_32 = ['Cactus_QP_32_depth1.csv']
train_QP_37 = ['Cactus_QP_37_depth1.csv']

max_depths = [5,1,8,1]
trains = [train_QP_22, train_QP_27, train_QP_32, train_QP_37]
qps = [22,27,32,37]

for train,max_depth,qp in zip(trains,max_depths,qps):

    data = Data()
    #if qp == 27 or qp==37:
    #    data.load_data(train,valid,ftk=[5,6,8])
    #else:
    data.load_data(train,train,ftk=[5,6,8])
    #if qp == 22:
    clf = Classifier(data,max_depth=max_depth)
    #else:
    #    clf = Classifier(data,max_depth=max_depth,hack=True)
    clf.fit_tree()
    clf.prune_duplicate_leaves(clf.clf)
    clf.get_stats()
    print('Acc: '+ str(clf.acc))
    print('Cost:' + str(clf.total_cost))
    print('Min Cost: ' + str(clf.calculate_minimal_cost()))
    print('Ratio: ' + str(clf.total_cost/clf.calculate_minimal_cost()))
    et = ExportTree(clf)
    et.write_tree_cpp(1,qp,3)
    print('\n')
