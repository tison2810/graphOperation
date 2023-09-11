//#include "common.h"

#ifndef _common_h_
#include "common.h"
#define _common_h_
#endif

#include <stack>
#include <queue>

int readFile(Graph &myGraph, char* filename )
{
    #define MAX_CHARACTER_PER_LINE 80
	char* file_name = filename;
	FILE* f = 0;
	
	f = fopen(file_name, "r");
	if (f == NULL)	//file not found || cannot read
		return 0;
	///////////////////////////////////////////////
	char	str[MAX_CHARACTER_PER_LINE];

	fgets(str, MAX_CHARACTER_PER_LINE, f);
	sscanf(str, "%d", &myGraph.nGraphType);

    fgets(str, MAX_CHARACTER_PER_LINE, f);
    sscanf(str, "%d", &myGraph.nOperation);

    fgets(str, MAX_CHARACTER_PER_LINE, f);
    sscanf(str, "%d", &myGraph.nVertexNum);

    fgets(str, MAX_CHARACTER_PER_LINE, f);

    // int end = 0;
    // int start = 0;
    // char substring[10];
    char *token = strtok(str, " ");
	for(int i = 0; i<myGraph.nVertexNum; i++){
        // while(str[end] != ' ' && str[end] != '\0'){
        //     end++;
        // }
        // strncpy(substring, str + start, end - start);
        // substring[end - start] = '\0';
        sscanf(token, "%s", myGraph.graph[i].vertex.strName);
        token = strtok(NULL, " ");
        // start = end + 1;
        // end = start;
	}

    fgets(str, MAX_CHARACTER_PER_LINE, f);
    // for(int i = 0; i<myGraph.nVertexNum; i++){
    //     if(myGraph.graph[i].vertex.strName == str){
    //         myGraph.startVertex = myGraph.graph[i].vertex;
    //         break;
    //     }
    // }
    sscanf(str, "%s", myGraph.startVertex.strName);
    // for(int i = 0; i<myGraph.nVertexNum; i++){
    //     if(strcmp(myGraph.graph[i].vertex.strName, myGraph.startVertex.strName) == 0){
    //         myGraph.startVertex = myGraph.graph[i].vertex;
    //         break;
    //     }
    // }

    fgets(str, MAX_CHARACTER_PER_LINE, f);
    sscanf(str, "%d", &myGraph.nOrder);

    // char *s;
    // float weight;
    for(int i = 0; i < myGraph.nVertexNum; i++){
        fgets(str, MAX_CHARACTER_PER_LINE, f);
        token = strtok(str, " ");
        // int end = 0;
        // int start = 0;
        for(int j = 0; j < myGraph.nVertexNum; j++){
            float weight;
            // while(str[end] != ' ' && str[end] != '\0'){
            //     end++;
            // }
            // strncpy(s, str + start, end - start);
            sscanf(token, "%f", &weight);
            if(weight != 0){
                VertexType *Slot = new VertexType();
                strcpy(Slot->strName, myGraph.graph[j].vertex.strName);
                Slot->weight = weight;
                if(myGraph.graph[i].adjVertex == NULL)
                    myGraph.graph[i].adjVertex = Slot;
                else{
                    VertexType *temp = myGraph.graph[i].adjVertex;
                    while (temp->next != NULL)
                        temp = temp->next;
                    temp->next = Slot;
                }
                //Update Indegree and Outdegree
                for(int k = 0; k < myGraph.nVertexNum; k++){
                    if(strcmp(Slot->strName, myGraph.graph[k].vertex.strName) == 0){
                        myGraph.graph[k].indegree++;
                        break;
                    }
                }
                myGraph.graph[i].outdegree++;
            }
            token = strtok(NULL, " ");
            // start = end + 1;
            // end = start;
        }
    }
    
	///////////////////////////////////////////////
	fclose(f);

    return 1;
}

// bool vertexSwap(VertexType *prev, VertexType *cur){
//     if(prev != NULL)
//         prev->next = cur->next;
//     cur->next = cur->next->next;
//     cur->next->next = cur;
//     return true;
// }

void adjVertexSort(Graph &myGraph){
    if(myGraph.nOrder == 0){
        for(int i = 0; i < myGraph.nVertexNum; i++){
            VertexType *temp = myGraph.graph[i].adjVertex;
            if(temp != NULL){
                // VertexType *slow;
                bool flag = true;
                while(flag){
                    VertexType *fast = temp;
                    flag = false;
                    while(fast->next != NULL){
                        if (strcmp(fast->strName, fast->next->strName) > 0){
                            flag = true;
                            swap(fast->strName, fast->next->strName);
                            swap(fast->weight, fast->next->weight);
                        }
                        fast = fast->next;
                    }
                }
            }
        }
    }    
    else{
        for(int i = 0; i < myGraph.nVertexNum; i++){
            VertexType *temp = myGraph.graph[i].adjVertex;
            // VertexType *slow;
            if(temp != NULL){
                bool flag = true;
                while(flag){
                    VertexType *fast = temp;
                    flag = false;
                    while(fast->next != NULL){
                        if (strcmp(fast->strName, fast->next->strName) < 0){
                            flag = true;
                            swap(fast->strName, fast->next->strName);
                            swap(fast->weight, fast->next->weight);
                        }
                        fast = fast->next;
                    }
                }
            }
        }
    }
}

void markVisited(Graph &myGraph, VertexType temp){
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(strcmp(temp.strName, myGraph.graph[i].vertex.strName) == 0){
            myGraph.graph[i].isMarked = true;
            myGraph.graph[i].vertex.isMarked = true;
        }
        VertexType *tmp = myGraph.graph[i].adjVertex;
        while(tmp != NULL){
            if(strcmp(tmp->strName, temp.strName) == 0)
                tmp->isMarked = true;
            tmp = tmp->next;
        }
    }

}

void DFSRec(Graph &myGraph, stack<VertexType> &L, VertexType startVertex, string &s){
    L.pop();
    VertexType *temp;
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(strcmp(startVertex.strName, myGraph.graph[i].vertex.strName) == 0){
            temp = myGraph.graph[i].adjVertex;
            break;
        }
    }
    while(temp != NULL){
        if(temp->isMarked == false){
            L.push(*temp);
            markVisited(myGraph, *temp);
        }
        temp = temp->next;
    }
    s += string(startVertex.strName) +" ";
    if(!L.empty())
        DFSRec(myGraph, L, L.top(), s);
    return;
}

void BFSRec(Graph &myGraph, queue<VertexType> &L, VertexType startVertex, string &s){
    L.pop();
    VertexType *temp;
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(strcmp(startVertex.strName, myGraph.graph[i].vertex.strName) == 0){
            temp = myGraph.graph[i].adjVertex;
            break;
        }
    }
    while(temp != NULL){
        if(temp->isMarked == false){
            L.push(*temp);
            markVisited(myGraph, *temp);
        }
        temp = temp->next;
    }
    s += string(startVertex.strName) +" ";
    if(!L.empty())
        BFSRec(myGraph, L, L.front(), s);
    return;    
}

void DTSRec(Graph &myGraph, stack<VertexType> &L, VertexType startVertex, string &s){
    int outDeg;
    int k;

    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(strcmp(startVertex.strName, myGraph.graph[i].vertex.strName) == 0){
            outDeg = myGraph.graph[i].outdegree;
            k = i;
            break;
        }
    }

    if(outDeg == 0){
        s = string(startVertex.strName) + " " + s;
        L.pop();
        return;
    }

    VertexType *temp;
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(strcmp(startVertex.strName, myGraph.graph[i].vertex.strName) == 0){
            temp = myGraph.graph[i].adjVertex;
            break;
        }
    }
    while(temp != NULL){
        if(temp->isMarked == false){
            L.push(*temp);
            markVisited(myGraph, *temp);
            DTSRec(myGraph, L, L.top(), s);
        }
        outDeg--;
        myGraph.graph[k].outdegree = outDeg;
        temp = temp->next;
    }
    if(outDeg == 0){
        s = string(startVertex.strName) + " " + s;
        L.pop();
        return;
    }    
    // if(!L.empty())
    //     DTSRec(myGraph, L, L.top(), s);
}

void BTSRec(Graph &myGraph, queue<VertexType> &L, VertexType startVertex, string &s){
    L.pop();
    VertexType *temp;
    int k; 
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(strcmp(startVertex.strName, myGraph.graph[i].vertex.strName) == 0){
            temp = myGraph.graph[i].adjVertex;
            k = i;
            break;
        }
    }
    while(temp != NULL){
        if(temp->isMarked == false){
            for(int i = 0; i < myGraph.nVertexNum; i++){
                if(strcmp(temp->strName, myGraph.graph[i].vertex.strName) == 0){
                    myGraph.graph[i].indegree--;
                    if(myGraph.graph[i].indegree == 0){
                        L.push(*temp);
                        markVisited(myGraph, *temp);
                    }
                    break;
                }
            }
        }
        temp = temp->next;
    }
    s += string(startVertex.strName) +" ";
    if(!L.empty())
        BTSRec(myGraph, L, L.front(), s);
    return;    
}

string depthFirstTraversal(Graph &myGraph, VertexType startVertex)
{
    adjVertexSort(myGraph);
    string result="";
    stack<VertexType> waitList;
    markVisited(myGraph, startVertex);
    waitList.push(startVertex);
    DFSRec(myGraph, waitList, startVertex, result);
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(myGraph.graph[i].isMarked == false){
            markVisited(myGraph, myGraph.graph[i].vertex);
            waitList.push(myGraph.graph[i].vertex);
            DFSRec(myGraph, waitList, myGraph.graph[i].vertex, result);
        }
    }    
    result[result.length() - 1]='\0';   
    return result;
}

string breadthFirstTraversal(Graph &myGraph, VertexType startVertex)
{
    adjVertexSort(myGraph);
    string result="";
    queue<VertexType> waitList;
    markVisited(myGraph, startVertex);
    waitList.push(startVertex);
    BFSRec(myGraph, waitList, startVertex, result);
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(myGraph.graph[i].isMarked == false){
            markVisited(myGraph, myGraph.graph[i].vertex);
            waitList.push(myGraph.graph[i].vertex);
            BFSRec(myGraph, waitList, myGraph.graph[i].vertex, result);
        }
    }
    result[result.length() - 1]='\0';   
    return result;
}

string depthTopoSort(Graph &myGraph) //stack + out
{
    adjVertexSort(myGraph);
    string result="";
    stack<VertexType> waitList;
    markVisited(myGraph, myGraph.startVertex);
    waitList.push(myGraph.startVertex);
    DTSRec(myGraph, waitList, myGraph.startVertex, result);
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(myGraph.graph[i].isMarked == false){
            markVisited(myGraph, myGraph.graph[i].vertex);
            waitList.push(myGraph.graph[i].vertex);
            DTSRec(myGraph, waitList, myGraph.graph[i].vertex, result);
        }
    }
    result[result.length() - 1]='\0';   
    return result;
}

string breadthTopoSort(Graph &myGraph) //queue + in
{
    adjVertexSort(myGraph);
    string result="";
    queue<VertexType> waitList;
    for(int i = 0; i < myGraph.nVertexNum; i++){
        if(myGraph.graph[i].indegree == 0){
            waitList.push(myGraph.graph[i].vertex);
            markVisited(myGraph, myGraph.graph[i].vertex);
        }
    }
    BTSRec(myGraph, waitList, waitList.front(), result);
    result[result.length() - 1]='\0';   
    return result;
}

string minSpanTree(Graph &myGraph, VertexType startVertex)
{
    int sum = 0;
    int min = INT_MAX;
    vector<VertexType> List;
    List.push_back(startVertex);
    VertexType *temp, *r;
    
    for(int i = 0; i < myGraph.nVertexNum; i++) {
        if(strcmp(startVertex.strName, myGraph.graph[i].vertex.strName) == 0)
            temp = myGraph.graph[i].adjVertex;
    }
    while(temp != NULL){
        if(temp->weight < min){
            min = temp->weight;
            r = temp;
        }
        temp = temp->next;
    }
    sum += min;
    r->weight = INT_MAX;
    for(int i = 0; i < myGraph.nVertexNum; i++) {
        if(strcmp(r->strName, myGraph.graph[i].vertex.strName) == 0){
            List.push_back(myGraph.graph[i].vertex);
            markVisited(myGraph, myGraph.graph[i].vertex);
        }
    }

    bool flag = true;
    while(flag){
        min = INT_MAX;
        flag = false;
        for(int j = 0; j < myGraph.nVertexNum; j++){
            if(myGraph.graph[j].isMarked == false){
                flag = true;
                break;
            }
        }
        for(int k = 0; k < List.size(); k++){
            if(List.size() == myGraph.nVertexNum)
                break;
            for(int i = 0; i < myGraph.nVertexNum; i++){
                if(strcmp(List[k].strName, myGraph.graph[i].vertex.strName) == 0){
                    temp = myGraph.graph[i].adjVertex;
                }
            }
            while(temp != NULL){
                bool check = true;
                for(int j = 0; j < List.size(); j++)
                    if(strcmp(temp->strName, List[j].strName) == 0){
                        check = false;
                        break;
                    }
                    if((temp->weight < min) && check){
                        min = temp->weight;
                        r = temp;
                    }
                temp = temp->next;
            }
        }
        if(List.size() == myGraph.nVertexNum)
                flag = false;
        else{
            sum += min;
            r->weight = INT_MAX;
            for(int i = 0; i < myGraph.nVertexNum; i++) {
                if(strcmp(r->strName, myGraph.graph[i].vertex.strName) == 0){
                    List.push_back(myGraph.graph[i].vertex);
                    markVisited(myGraph, myGraph.graph[i].vertex);
                }
            }
        }
    }
    string result = to_string(sum);
    return result;
}
