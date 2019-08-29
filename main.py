import scipy.io
import pandas as pd
import numpy as np
from sklearn.tree import DecisionTreeClassifier
from sklearn import tree
from sklearn.model_selection import train_test_split
from sklearn.tree.export import export_text
import pydotplus
from IPython.display import Image
import bib

data_d0 = scipy.io.loadmat('alldata_depth0.mat')
data = pd.DataFrame(data_d0['data'])

qp = data_d0['qp']
qp_out = [q[0] for q in qp]
qp = pd.DataFrame(qp_out)

features = data.iloc[:,0:10]
classes = data.iloc[:,10]
costs = pd.read_csv('costs.csv',sep='|')

X = features
y = classes

X_train, X_test, y_train, y_test, costs_train, costs_test, qp_train, qp_test = train_test_split(X, y, costs, qp, test_size=0.2, random_state=42)

c = 0
acc = 0
clfs = []
for qp_val in [22,27,32,37]:
    X_train_o, X_test_o, y_train_o, y_test_o, costs_train_o, costs_test_o = bib.separate_qp(qp_val, qp_train, qp_test, X_train, X_test, y_train, y_test, costs_train, costs_test)
    clf = DecisionTreeClassifier(max_depth=3,random_state=42)
    clf.fit(X_train_o,y_train_o)
    y_pred_DT = clf.predict(X_test_o)
    acc = acc + clf.score(X_test_o,y_test_o)
    c = c + bib.calculate_cost_of_decisions(y_pred_DT,costs_test_o)
    clfs.append(clf)

r = export_text(clfs[0])

fo = open('tree.txt','w')
fo.write(r)
fo.close()

# Create DOT data
class_names = np.array(['0','1'], dtype='<U10')
dot_data = tree.export_graphviz(clfs[0], out_file=None, class_names=['0','1'])
# Draw graph
graph = pydotplus.graph_from_dot_data(dot_data)

# Create PNG
graph.write_png("tree.png")
