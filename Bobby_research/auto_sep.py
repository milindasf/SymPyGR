import expressionTree as eTree
import disjoint_set as djs
import fileParser as fP

import sys


def cached_subtrees(fname,cache):
        tree = eTree.expressionTree()
        id =0
        for line in open('bssn.cpp'):
                if not line.startswith('//') and line != '\n':
                        parse = fP.expressionLine(line,id)
                        #print(parse.tokens)
                        tree, _ = parse.createTree(tree)
                        id = parse.id
        
        #while cache <=95:
        subtrees = tree.cacheAdaptTrees(cache)
        return subtrees
        


def stage_trees(subtrees,cache=100):

        loop_begin ='\nfor (unsigned int k = 3; k < nz-3; k++) {\n'
        loop_begin = loop_begin + '   const double z = pmin[0] + k*hz;\n'
        loop_begin = loop_begin + '    for (unsigned int j = 3; j < ny-3; j++) {\n'
        loop_begin = loop_begin + '            const double y = pmin[0] + j*hy;\n'                
        loop_begin = loop_begin + '        for (unsigned int i = 3; i < nx-3; i++) {\n'
        loop_begin = loop_begin + '            const double x = pmin[0] + i*hx;\n'
        loop_begin = loop_begin + '            const double r_coord = sqrt(x*x + y*y + z*z);\n'
        loop_begin = loop_begin + '            double eta=ETA_CONST;\n'
        loop_begin = loop_begin + '            if (r_coord >= ETA_R0) {\n'
        loop_begin = loop_begin + '                eta *= pow( (ETA_R0/r_coord), ETA_DAMPING_EXP);\n'
        loop_begin = loop_begin + '            }\n'
        loop_begin = loop_begin + '            const unsigned int pp = i + nx*(j + ny*k);\n'

        loop_end = "\n                            }\n"
        loop_end = loop_end + "           }\n"
        loop_end = loop_end + "    }\n"
        
        # allocate block local vars. 
        for subtree in subtrees:
                for source in subtree.sources:
                        if source.startswith('_') or source.startswith("DENDRO"):
                                alloc_str = "double* " + source + "= (double*)malloc(sizeof(double)*n);"
                                print(alloc_str)



        num_dep = list()
        for subtree in subtrees:
                for source in subtree.sources:
                        num_dep.append(subtree.getNumLeafDependents(source))


        # compute code. 
        dep =0
        count =0
        lb = False
        passes = 0
        for subtree in subtrees:
                for source in subtree.sources:
                        count = count + 1
                        if(subtree.getNumLeafDependents(source) >=2):
                                if(dep == 0 ):
                                        passes = passes + 1
                                        print("bssn::timer::t_rhs.start();")
                                        print(loop_begin)
                                        lb= True

                                dep = dep + subtree.getNumLeafDependents(source)

                                if not source.endswith('[pp]'):
                                        var_end = '[pp]'
                                        print("                  // unique dep: "+ str(subtree.getNumLeafDependents(source)))
                                        code_str =  "            " + source+ var_end + ' = ' + subtree.createCodeOutput(source)+ ';\n'
                                        print(code_str)
                                else:
                                        var_end = ''
                                        print("                  // unique dep: "+ str(subtree.getNumLeafDependents(source)))
                                        code_str =  "            " + source+ var_end + ' = ' + subtree.createCodeOutput(source)+ ';\n'
                                        print(code_str)
                                nCount = count 
                                while((nCount < len(num_dep)) and (num_dep[nCount] <2)):
                                        nCount = nCount + 1
                                
                                if((nCount < len(num_dep)) and (num_dep[nCount] >=2) and (dep + num_dep[nCount] ) > cache   ): 
                                        print(loop_end)
                                        print("                  // unique dep for stage : "+ str(dep))
                                        print("bssn::timer::t_rhs.stop();")
                                        dep = 0
                                        lb=False
                                
                if(lb):
                        print(loop_end)
                        print("                  // unique dep for stage : "+ str(dep))
                        print("bssn::timer::t_rhs.stop();")
                        dep = 0
                        lb=False

        print("// total number of passes : "+str(passes))
                                # dealloc block local vars. 
        for subtree in subtrees:
                for source in subtree.sources:
                        if source.startswith('_') or source.startswith("DENDRO"):
                                dealloc_str = 'free(' + source + ');'
                                print(dealloc_str)
                
        




def main():

        '''
        line =  fP.expressionLine("double DENDRO_972 = BSSN_ETA_R0*sqrt(DENDRO_41*(DENDRO_106*DENDRO_971 - DENDRO_274*DENDRO_55 - DENDRO_278*DENDRO_35 - DENDRO_280*DENDRO_49 - DENDRO_357*DENDRO_971 + 2*DENDRO_60*DENDRO_85))*pow(-pow(chi[pp], BSSN_ETA_POWER[0]) + 1, -BSSN_ETA_POWER[1]);")
        tree, id = line.createTree()
        tree.createGraphPicture('pictures/pow')
        '''



        
        tree = eTree.expressionTree()
        id =0
        for line in open('bssn.cpp'):
                if not line.startswith('//') and line != '\n':
                        parse = fP.expressionLine(line,id)
                        #print(parse.tokens)
                        tree, junk = parse.createTree(tree)
                        id = parse.id
        #tree.createGraphPicture('pictures/basicAdd')
        
        cache = int(sys.argv[1])
        #cache = 200
        passSize = cache

        print("// dependency threshold : "+ str(cache))

        subtrees = cached_subtrees("bssn.cpp",cache)
        stage_trees(subtrees,cache=cache)

        '''
        
        counter = 0
        #while cache <=95:
        subtrees = tree.cacheAdaptTrees(cache)
        
        file = open("staged_codes/" + "cache" + str(cache) + "/Code"+str(cache)+".cpp", "w")
        alloc_file = open("staged_codes/" + "cache" + str(cache) + "/allocate"+str(cache)+".cpp", "w")
        dealloc_file = open("staged_codes/" + "cache" + str(cache) + "/deallocate"+str(cache)+".cpp", "w")
        for subtree in subtrees:   
                #subtree.createGraphPicture('pictures/subT'+str(counter)) 
                #print(len(subtree.sources))       
                for source in subtree.sources: 
                        if subtree.getNumLeafDependents(source) >=2 : 
                                counter = counter + 1
                                
                                output = ''
                                var_end = ''
                                end = ''
                                
                                if counter % passSize == 0:                                        
                                        output = '\nfor (unsigned int k = 3; k < nz-3; k++) {\n'
                                        output = output + '    for (unsigned int j = 3; j < ny-3; j++) {\n'
                                        output = output + '        for (unsigned int i = 3; i < nx-3; i++) {\n'
                                        output = output + '            x = pmin[0] + i*hx;\n'
                                        output = output + '            r_coord = sqrt(x*x + y*y + z*z);\n'
                                        output = output + '            eta=ETA_CONST;\n'
                                        output = output + '            if (r_coord >= ETA_R0) {\n'
                                        output = output + '                eta *= pow( (ETA_R0/r_coord), ETA_DAMPING_EXP);\n'
                                        output = output + '            }\n'
                                        output = output + '            pp = i + nx*(j + ny*k);\n'
                                if counter % cache == cache -1:
                                        end = '        }\n'
                                        end = end + '    }\n'
                                        end = end + '}\n'

                                if not source.endswith('[pp]'):
                                        #output = 'double '
                                        var_end = '[pp]'
                                
                                output = output + '            ' + source+ var_end + ' = ' + subtree.createCodeOutput(source)+ ';\n' + end
                                file.write(output)

                                if source.startswith('_') or source.startswith("DENDRO"):
                                        
                                        alloc_str = "double* " + source + "= (double*)malloc(sizeof(double)*n);\n"
                                        alloc_file.write(alloc_str)
                                        
                                        dealloc_str = 'free(' + source + ');\n'
                                        dealloc_file.write(dealloc_str)

                                

                #counter = counter + 1
        end = '        }\n'
        end = end + '    }\n'
        end = end + '}\n'
        file.write(end)
        print('number stages ' + str(counter))
                #cache = cache +5
        '''
def calculate_rhs():

        a = 1
        K = 2
        b = [3,4,5]
        l1 = 6
        e_i = [0,1,2]
        ad = 7,8,9

        lie =  sum([b[i] * ad[i] for i in e_i]) 
        value = l1*lie - 2*a*K 

        symbols = {}
        symbols['a'] = a
        symbols['K'] = K
        symbols['l1'] = l1
        symbols['b0'] = b[0]
        symbols['b1'] = b[1]
        symbols['b2'] = b[2]
        symbols['ad(0,a)'] = ad[0]
        symbols['ad(1,a)'] = ad[1]
        symbols['ad(2,a)'] = ad[2]

        return value, symbols

def create_arhs():
        tree = eTree.expressionTree()
        
        #create left side term
        tree.addLeafNode(1,"-2")
        tree.addLeafNode(2,"a")
        tree.addLeafNode(3, "K")
        tree.addNonLeafNode(4,"*",2,3)
        tree.addNonLeafNode(5,"*",1,4)

        #implement tree for dendro.lie
        tree.addLeafNode(6,"b0")
        tree.addLeafNode(7,"b1")
        tree.addLeafNode(8,"b2")

        tree.addLeafNode(9,"ad(0,a)")
        tree.addLeafNode(10,"ad(1,a)")
        tree.addLeafNode(11,"ad(2,a)")

        tree.addNonLeafNode(12,"*",6,9)
        tree.addNonLeafNode(13,"*",7,10)
        tree.addNonLeafNode(14,"*",8,11)
        tree.addNonLeafNode(15,"+",12,13)
        tree.addNonLeafNode(16,"+",15,14)

        #create right side term
        tree.addLeafNode(0,"l1")
        tree.addNonLeafNode(17,"*",0,16)

        #merfe right side and left side
        tree.addNonLeafNode(18,"+", 17, 5)

        return tree

def run_add_tree_example():

    tree1 = createTree1()
    print(tree1.evaluateNode(20))
    tree1.createGraphPicture('kTree1')
    tree1.uniformStaging()
    print(tree1.evaluateNode(20))
    tree1.createGraphPicture('kTree1Stage')
    
    


    tree2 = createTree2()
    print(str(tree2.evaluateNode(49)) + " " + str(tree2.evaluateNode(50)))
    tree2.createGraphPicture('kTree2')
    tree2.uniformStaging()
    print(str(tree2.evaluateNode(49)) + " " + str(tree2.evaluateNode(50)))
    tree2.createGraphPicture('kTree2Stage')
    

    
    tree1.addExpresionTree(tree2)
    tree1.createGraphPicture('kMerge')
    tree1.uniformStaging()
    tree1.createGraphPicture('kMerge')
    

    subtrees = tree1.cacheAdaptTrees(5)

    cache = {}
    count = 0
    for subtree in subtrees:
        subtree.createGraphPicture("kMergeSubtree " + str(count))
        count = count + 1

        for source in subtree.sources:
            sourceValue = subtree.evaluateNode(source,cache)
            cache[source] = sourceValue
    print(str(cache[20]) + " " + str(cache[49]) + " " + str(cache[50]))

def createTree1():
    tree = eTree.expressionTree()

    #add leaf nodes
    tree.addLeafNode(0, 5)
    tree.addLeafNode(1, 5)
    tree.addLeafNode(2,  3)
    tree.addLeafNode(3,  4)
    tree.addLeafNode(4,  2)
    tree.addLeafNode(5,  1)
    tree.addLeafNode(6,  4)
    tree.addLeafNode(7,  2)
    tree.addLeafNode(8,  3)
    tree.addLeafNode(9,  5)
    tree.addLeafNode(10, 5)
    
    #add nonleaf nodes
    tree.addNonLeafNode(11, '+', 1, 2)
    tree.addNonLeafNode(12, '+', 3, 4)
    tree.addNonLeafNode(13, '+', 6, 7)
    tree.addNonLeafNode(14, '+', 9, 10)
    tree.addNonLeafNode(15, '+', 0, 11)
    tree.addNonLeafNode(16, '+', 5, 13)
    tree.addNonLeafNode(17, '+', 8, 14)
    tree.addNonLeafNode(18, '+', 12, 15)
    tree.addNonLeafNode(19, '+', 16, 17)
    tree.addNonLeafNode(20, '+', 18, 19)

    return tree

def createTree2():
    tree = eTree.expressionTree()

    #add leaf nodes
    tree.addLeafNode(30, 5)
    tree.addLeafNode(31, 5)
    tree.addLeafNode(32, 3)
    tree.addLeafNode(33, 4)
    tree.addLeafNode(34, 2)
    tree.addLeafNode(35, 1)
    tree.addLeafNode(36, 4)
    tree.addLeafNode(37, 2)
    tree.addLeafNode(38, 21)
    tree.addLeafNode(39, 19)

    #add non leaf nodes
    tree.addNonLeafNode(41, '+', 31, 32)
    tree.addNonLeafNode(42, '+', 33, 34)
    tree.addNonLeafNode(43, '+', 36, 37)
    tree.addNonLeafNode(45, '+', 30, 41)
    tree.addNonLeafNode(46, '+', 35, 43)
    tree.addNonLeafNode(44, '+', 38, 46)
    tree.addNonLeafNode(47, '+', 39, 44)
    tree.addNonLeafNode(48, '+', 42, 45)
    tree.addNonLeafNode(49, '+', 46, 47)
    tree.addNonLeafNode(50, '+', 47, 48)

    return tree

def create_mult_tree():
    
    tree = eTree.expressionTree()
    tree.addLeafNode(0,2)
    tree.addLeafNode(1,5)
    tree.addLeafNode(2,2)

    
    tree.addLeafNode(3,4)
    tree.addLeafNode(4,5)
    
    tree.addLeafNode(5,2)
    tree.addLeafNode(6,4)
    tree.addLeafNode(7,5)
    tree.addLeafNode(8,2)
    
    tree.addLeafNode(9,5)
    tree.addLeafNode(10,4)
    

    tree.addNonLeafNode(11,"+",0,1)
    tree.addNonLeafNode(12,"*",2,11)
    
    tree.addNonLeafNode(13,"*",4,3)
    tree.addNonLeafNode(14,"+",13,12)
    
    tree.addNonLeafNode(15,"+",6,5)
    tree.addNonLeafNode(16,"+",8,7)
    tree.addNonLeafNode(17,"*",16,15)
    tree.addNonLeafNode(18,"+",17,14)
    
    tree.addNonLeafNode(19,"*",10,9)
    
    tree.addNonLeafNode(20,"*",19,18)
    

    return tree

def disjoint_set():
    dj = djs.disjoint_set()
    dj.add(5)
    dj.add(6)
    dj.add(7)
    dj.add(8)
    dj.add(9)
    dj.add(10)

    dj.union(5,6)
    dj.union(7,8)
    dj.union(9,10)
    dj.union(10,6)

    print(dj.find(10))
    print(dj.find(9))
    print(dj.find(8))
    print(dj.find(7))
    print(dj.find(6))
    print(dj.find(5))

if __name__ == '__main__':
    main()