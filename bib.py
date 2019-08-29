import pandas as pd
import numpy as np
import re

def calculate_cost_of_decisions(y_pred, costs):
    cost = 0
    for i in range(len(y_pred)):
        if(y_pred[i] == 0):
            cost = cost + costs.iloc[i,0]
        else:
            cost = cost + costs.iloc[i,1]
    return cost

def separate_qp(qp_val, qp_train, qp_test, X_train, X_test, y_train, y_test, costs_train, costs_test):
    qp_set =[]
    if qp_val == 22:
        qp_set = [23,24,25]
    elif qp_val == 27:
        qp_set = [28,29,30]
    elif qp_val == 32:
        qp_set = [33,34,35]
    elif qp_val == 37:
        qp_set = [38,39,40]

    idxs0 = np.where(qp_train == qp_set[0])[0]
    idxs1 = np.where(qp_train == qp_set[1])[0]
    idxs2 = np.where(qp_train == qp_set[2])[0]

    idxs = np.concatenate((idxs0,idxs1,idxs2))

    X_train_o = X_train.iloc[idxs,:]
    y_train_o = y_train.iloc[idxs]
    costs_train_o = costs_train.iloc[idxs,:]

    idxs0 = np.where(qp_test == qp_set[0])[0]
    idxs1 = np.where(qp_test == qp_set[1])[0]
    idxs2 = np.where(qp_test == qp_set[2])[0]

    idxs = np.concatenate((idxs0,idxs1,idxs2))

    X_test_o = X_test.iloc[idxs,:]
    y_test_o = y_test.iloc[idxs]
    costs_test_o = costs_test.iloc[idxs,:]

    return X_train_o, X_test_o, y_train_o, y_test_o, costs_train_o, costs_test_o

def isLeaf(line):
    return len(re.findall('class',line)) != 0

def get_ntabs(line):
    return len(re.findall(r'\|   ',line))

def isIf(line):
    return len(re.findall(r'<=',line)) != 0

def getIf(line):
    n_tabs = get_ntabs(line)
    feature = re.findall(r'_\d',line)[0][1]
    val = re.findall(r'\d+.\d+',line)[0]
    return '    '*n_tabs + 'if(dFeatures[' + feature + '] <= ' + val + ')\n'

def getClass(line):
    return re.findall(r'class: (\d).\d',line)[0]


def get_cpp_code(filename):

    f1 = open('tree.txt','r')
    lines = f1.read()
    lines = lines.split('\n')
    lines = lines[:-1]
    f1.close()

    fo = open('tree.cpp','w')

    i=0 #iterador do loop
    n_lines = len(lines) #numero de linhas no arquivo
    for line in lines:
        #se a linha for um 'if'
        if(isIf(line)):
            #escreve a if sentence no arquivo
            fo.write(getIf(line))
            #somente abre chave depois do if se a próxima linha não for uma folha
            if(i < n_lines and not isLeaf(lines[i+1])):
                fo.write('    '*get_ntabs(line)+'{\n')
        #se a linha for uma folha, so escreve o return
        elif (isLeaf(line)):
            fo.write('    '*get_ntabs(line) + 'return ' + getClass(line) + ';\n')
        #se a linha for um else
        elif (i < n_lines):
            #se as linhas anteriores e proxima são leafs, não coloco chave alguma
            if(isLeaf(lines[i-1]) and isLeaf(lines[i+1])):
                fo.write('    '*get_ntabs(line) + 'else\n')
            #caso contrário, fecho chaves antes do else e abro depois dele
            else:
                fo.write('    '*(get_ntabs(lines[-1])-2) + '}\n')
                fo.write('    '*get_ntabs(line) + 'else\n')
                fo.write('    '*get_ntabs(line) + '{\n')
        i+=1
    #fecha a última chave que faltou
    fo.write('    '*(get_ntabs(lines[-1])-2) + '}')
    fo.close()
