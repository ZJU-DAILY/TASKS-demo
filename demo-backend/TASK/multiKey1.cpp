//
// Created by 罗程阳 on 2022/9/13.
//

#include "util/ioPB.h"
#include "util/ioUint.h"
#include "util/trie.h"
#include "util/util.h"
#include "util/mtrie.h"
#include "util/canLabelET.h"
#include <climits>
#include <time.h>
#include <malloc.h>

#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#define MAXIMUM 0x7fffffff
#define MYPORT  1234
#define BUFFER_SIZE 1024

using namespace std;

string delimiter = ",", space = " ";

char *txtName, *labelForwardName, *PBName, *degName;
string outName, pathName;
Trie *trie;
vector<string> recs;
vector<string> queries;
int vertexNum;
unordered_map<int, vector<int>> keyVertex;  // key id -> vertex id
unordered_map<int, vector<int>> vertexNode;  // vertex id -> node id
unordered_map<int, vector<edgeS>> labelForward;
unordered_map<int, PB> vertexPB;  // vertex id and its corresponding Prefix Backward (PB)

string testFileName;
vector<pair<int, int>> testCase;

// query related variables
vector<TrieNode*> topActiveNodes;  // active nodes at the first level
set<int> vertexes;
set<label> candidateLabel;
vector<set<int>> keyIds;
vector<int> queryPointLabel;

MTrie *mtrie;  // compact tree of active nodes
unordered_map<int, int> activeDepth;  // the depth of active node
unordered_map<int, int> activeUp;  // the Up if bound of active node
vector<MTrieNode*> trash;
int queryVertex;
int k;
int tau; // the threshold of edit distance
double alpha = 0.5;
double dMax = 100;
vector<vector<int>> PEDList;
int minPED;
vector<int> minPEDs;  // minPED corresponding to each keyword
vector<edgeS> queryLabelForward;
set<labelScore> Q;  // priority queue
set<labelScore> result;
vector<canLabelET> canLabels;
int Li = 0;  // the number of vertex that should be extended in the backward list
vector<string> input;
int inputNum = 0;
int keywordNum = 0;

int total_strLen = 0;

// ------------------ multikey
vector<string> kws;
int queryStringNum;


void LoadLabelForward() {
    ifstream outFile(outName, ios::binary | ios::out);
    outFile.read((char *)(&vertexNum), sizeof(vertexNum));
    for (int i = 0; i < vertexNum; i++) {
        int labelNum;
        outFile.read((char *)(&labelNum), sizeof(labelNum));
        for (int j = 0; j < labelNum; j++) {
            int vid, dis;
            outFile.read((char *) (&vid), sizeof(vid));
            outFile.read((char *) (&dis), sizeof(dis));
            labelForward[i].push_back(edgeS(vid, dis));
        }
        vector<edgeS>(labelForward[i]).swap(labelForward[i]);
    }
    outFile.close();
}

void LoadPB() {
    PBName = (char *) malloc(50);
    sprintf(PBName, "%s.PB", txtName);
    inBufPB inPB(PBName);
    int numPB;
    inPB.nextInt(numPB);
    for (int i = 0; i < numPB; i++) {
        if (i % 10000 == 0) {
            printf("%d/%d\n", i, numPB);
        }
        int vertexId;
        inPB.nextInt(vertexId);
        vertexPB[vertexId].vertexId = vertexId;
        inPB.getBackwardList(vertexPB[vertexId].backwardList);
        inPB.getNodeSet(vertexPB[vertexId].nodeSet);
        inPB.getBitMaps(vertexPB[vertexId].bitMaps);
        inPB.getNodeBitMapId(vertexPB[vertexId].nodeBitMapId);
    }
}

void LoadVertexNode() {
    // read in the keyVertex file, construct the keyVertex map
    ifstream inputKeyVertex(string(txtName) + ".keyVertex", ios::in);
    int keyNum;
    inputKeyVertex >> keyNum;
    int tempKeyId, tempVertexNum;
    inputKeyVertex >> tempKeyId;  // skip the number of keyword occurrence
    while (keyNum--) {
        inputKeyVertex >> tempKeyId >> tempVertexNum;
        vector<int> tempVertexList;
        for(int i = 0; i < tempVertexNum; i++) {
            int tempVertexId;
            inputKeyVertex >> tempVertexId;
            tempVertexList.push_back(tempVertexId);
        }
        keyVertex[tempKeyId] = tempVertexList;
    }
    inputKeyVertex.close();

    // construct the vertexNode
    for (int i = 0; i < trie->ids.size(); i++) {
        pair<int, int> tempId = trie->ids[i];
        int nodeId = tempId.first;
        int keyId = tempId.second;
        vector<int> vertexList = keyVertex[keyId];
        for (int j = 0; j < vertexList.size(); j++) {
            int tempVertexId = vertexList[j];
            if (vertexNode.count(tempVertexId) == 0) {
                vector<int> tempVertexNode;
                tempVertexNode.push_back(nodeId);
                vertexNode[tempVertexId] = tempVertexNode;
            } else {
                vertexNode[tempVertexId].push_back(nodeId);
            }
        }
    }
}

bool comp_id(const TrieNode* n, const int id) {
    return n->id < id;
}

bool comp_node(const TrieNode* t1, const TrieNode* t2) {
    if (t1->id < t2->id)
        return true;
    else if (t1->id > t2->id)
        return false;
    else
        return t1->last > t2->last;
}

void quickFind(vector<TrieNode*>::iterator &vit, const vector<TrieNode*>::const_iterator end, MTrieNode* node, int themin, const int row) {
    if (node->opt.first < 0) node->opt = node->parent->opt;
    MTrieNode* cnode = node->child;
    MTrieNode* pnode = NULL;

    int next_start = MAXIMUM;
    if (cnode != NULL) next_start = cnode->tnode->id;

    while (vit != end && (*vit)->id <= node->tnode->last) {
        if ((*vit)->id < next_start && (*vit)->last < next_start) {
            // left
            if (themin <= tau &&
                node->opt.second + (*vit)->depth - node->opt.first > themin) {
                MTrieNode* new_node = new MTrieNode();
                new_node->parent = node;
                new_node->tnode = *vit;
                new_node->eds.push_back(make_pair(row, themin));
                new_node->opt = make_pair((*vit)->depth, themin);
                if (pnode == NULL) {
                    node->child = new_node;
                    new_node->next = cnode;
                } else {
                    pnode->next = new_node;
                    new_node->next = cnode;
                }
                pnode = new_node;
            }
        } else if ((*vit)->id < next_start && (*vit)->last >= next_start) {
            // ancestor
            if (themin <= tau &&
                node->opt.second + (*vit)->depth - node->opt.first > themin) {
                MTrieNode* new_node = new MTrieNode();
                new_node->parent = node;
                new_node->child = cnode;
                new_node->tnode = *vit;
                new_node->eds.push_back(make_pair(row, themin));
                new_node->opt = make_pair((*vit)->depth, themin);
                while (true) {
                    cnode->parent = new_node;
                    if (cnode->next == NULL) {
                        cnode = cnode->next;
                        break;
                    }
                    if (cnode->next->tnode->id > (*vit)->last) {
                        MTrieNode* temp = cnode->next;
                        cnode->next = NULL;
                        cnode = temp;
                        break;
                    }
                    cnode = cnode->next;
                }
                if (pnode == NULL) {
                    node->child = new_node;
                } else {
                    pnode->next = new_node;
                }
                pnode = new_node;
                new_node->next = cnode;
            } else {
                while (cnode) {
                    if (cnode->tnode->id > (*vit)->last) break;
                    pnode = cnode;
                    cnode = cnode->next;
                }
            }
            if (cnode != NULL) next_start = cnode->tnode->id;
            else next_start = MAXIMUM;
        } else if ((*vit)->id == next_start) {
            // self
            if (themin <= tau &&
                node->opt.second + (*vit)->depth - node->opt.first > themin) {
                cnode->eds.push_back(make_pair(row, themin));
            }
        } else if ((*vit)->id > next_start && (*vit)->id <= cnode->tnode->last) {
            // child
            int thenewmin = themin;
            for (auto qit = cnode->eds.begin(); qit != cnode->eds.end(); ++qit) {  // <row, ed>
                if (row == qit->first) continue;
                if (thenewmin > qit->second + max(row - 1 - qit->first, (*vit)->depth - 1 - cnode->tnode->depth))
                    thenewmin = qit->second + max(row - 1 - qit->first, (*vit)->depth - 1 - cnode->tnode->depth);
            }
            if (thenewmin > tau)
                cout << "BIGGER BIGGER" << endl;
            quickFind(vit, end, cnode, thenewmin, row);
            pnode = cnode;
            cnode = cnode->next;
            if (cnode != NULL) next_start = cnode->tnode->id;
            else next_start = MAXIMUM;
            continue;
        } else if ((*vit)->id > next_start && (*vit)->id > cnode->tnode->last) {
            // right
            pnode = cnode;
            cnode = cnode->next;
            if (cnode != NULL) next_start = cnode->tnode->id;
            else next_start = MAXIMUM;
            continue;
        }
        ++vit;
    }
}

bool comp_lower(const pair<int, int> & p, const int v) {
    return p.first < v;
}

void getActiveNods() {
    for (int pos = 1; pos <= inputNum; pos++) {
        queue<MTrieNode *> q;  // mnode
        char ch = input[keywordNum][pos];     // current key stroke
        PEDList[keywordNum].assign(trie->root->last + 1, 999);
        minPED = 999;

        // Get the list of Mnodes;
        vector<pair<MTrieNode *, pair<int, int>>> v;  // all the mnodes <first_level, last_level>, BFS;
        queue<pair<MTrieNode *, int>> qp;  // <mnode, last_level>
        qp.push(make_pair(mtrie->root, -1));
        while (!qp.empty()) {
            qp.front().first->opt = make_pair(-32767, 0);
            int parent_last_level = qp.front().second;
            MTrieNode *cnode = qp.front().first->child;
            qp.pop();
            while (cnode) {
                int first_level = max(cnode->tnode->depth + 1, parent_last_level + 1);
                int last_level = cnode->tnode->depth + 1;
                for (; last_level <= cnode->tnode->depth + tau + 1; ++last_level) {
                    int themin = tau + 1;
                    for (auto qit = cnode->eds.begin(); qit != cnode->eds.end(); ++qit) {  // <row, ed>
                        if (themin > qit->second + max(pos - 1 - qit->first, last_level - 1 - cnode->tnode->depth))
                            themin = qit->second + max(pos - 1 - qit->first, last_level - 1 - cnode->tnode->depth);
                    }
                    if (themin > tau) break;
                }
                v.push_back(make_pair(cnode, make_pair(first_level, last_level - 1)));
                qp.push(make_pair(cnode, last_level - 1));
                cnode = cnode->next;
            }
        }

        // printf("%d\n", (int)v.size());

        for (auto pmnode = v.begin(); pmnode != v.end(); ++pmnode) {
            MTrieNode *node = pmnode->first;
            for (int depth = pmnode->second.first; depth <= pmnode->second.second; ++depth) {
                vector<TrieNode *> &vec = trie->index[ch][depth];
                auto vit = lower_bound(vec.begin(), vec.end(), node->tnode->id, comp_id);
                int themin = tau + 1;
                for (auto qit = node->eds.begin(); qit != node->eds.end(); ++qit) {  // <row, ed>
                    if (pos == qit->first) continue;
                    if (themin > qit->second + max(pos - 1 - qit->first, depth - 1 - node->tnode->depth))
                        themin = qit->second + max(pos - 1 - qit->first, depth - 1 - node->tnode->depth);
                }
                quickFind(vit, vec.end(), node, themin, pos);
            }
        }

        q.push(mtrie->root);
        while (!q.empty()) {
            MTrieNode *node = q.front();
            MTrieNode *cnode = node->child;
            q.pop();
            MTrieNode *pnode = NULL;
            while (cnode) {
                if (cnode->eds.back().first == pos) {
                    pair<int, int> match = cnode->eds.back();
                    cnode->eds.pop_back();
                    while (!cnode->eds.empty() && match.second <= cnode->eds.back().second)
                        cnode->eds.pop_back();
                    cnode->eds.push_back(match);
                }
                int pop_num = 0;
                int minPEDNow = INT32_MAX;
                for (auto qit = cnode->eds.begin(); qit != cnode->eds.end(); ++qit) {
                    if (pos - qit->first + qit->second > tau ||
                        node->opt.second + cnode->tnode->depth - node->opt.first <= qit->second)
                        ++pop_num;
                    if (pos - qit->first + qit->second < minPEDNow)
                        minPEDNow = pos - qit->first + qit->second;
                }
                if (pop_num == cnode->eds.size()) {
                    // remove the cnode
                    trash.push_back(cnode);
                    MTrieNode *ccnode = cnode->child;
                    if (ccnode == NULL) {
                        if (pnode == NULL) {
                            node->child = cnode->next;
                        } else {
                            pnode->next = cnode->next;
                            // TODO delete the pointer
                        }
                        cnode = cnode->next;
                    } else {
                        while (true) {
                            ccnode->parent = node;
                            if (ccnode->next == NULL) break;
                            ccnode = ccnode->next;
                        }
                        ccnode->next = cnode->next;
                        if (pnode == NULL) {
                            node->child = cnode->child;
                        } else {
                            pnode->next = cnode->child;
                        }
                        cnode = cnode->child;
                    }
                } else {
                    cnode->minPED = minPEDNow;
                    while (--pop_num >= 0) cnode->eds.pop_front();
                    // update the minPED list
                    if (cnode->minPED < cnode->parent->minPED) {
                        for (int i = cnode->tnode->id; i <= cnode->tnode->last; i++) {
                            PEDList[keywordNum][i] = minPEDNow;
                        }
                    }
                    if (minPEDNow < minPED)
                        minPED = minPEDNow;
                    q.push(cnode);
                    pnode = cnode;
                    cnode = cnode->next;
                }
            }
        }
        for (int i = 0; i < minPEDs.size(); i++) {
            minPED = min(minPED, minPEDs[i]);
        }
        topActiveNodes.clear();
        MTrieNode *tempNode = mtrie->root->child;
        while (tempNode != NULL) {
            topActiveNodes.push_back(tempNode->tnode);
            activeDepth[tempNode->tnode->id] = tempNode->tnode->depth;
            activeUp[tempNode->tnode->id] = tempNode->tnode->last;
            tempNode = tempNode->next;
        }
        sort(topActiveNodes.begin(), topActiveNodes.end(), comp_node);

        keyIds[keywordNum].clear();
        int prev_last = -1;
        auto tit = trie->ids.begin();
        for (auto vit = topActiveNodes.begin(); vit != topActiveNodes.end(); vit++) {
            if ((*vit)->last <= prev_last) continue;
            prev_last = (*vit)->last;
            tit = lower_bound(tit, trie->ids.end(), (*vit)->id, comp_lower);
            while (tit != trie->ids.end() && tit->first <= (*vit)->last) {
                keyIds[keywordNum].insert(tit->second);
                ++tit;
            }
        }
    }
}

double total_time = 0;
double extend_time = 0;
double cal_time = 0;

void CalScore(int rowNum) {
    timer tm;
    tm.restart();
    edgeS tempLabelForward = queryLabelForward[rowNum];
    PB *nowPB = &vertexPB[tempLabelForward.x];
    while(!canLabels[rowNum].labelPosList.empty()) {
        auto it = canLabels[rowNum].labelPosList.begin();
        if (it->pos >= nowPB->backwardList.size())
            break;
        edgeS tempLabel = nowPB->backwardList[it->pos];
        double SRD = (double)(tempLabelForward.w + tempLabel.w) / dMax; // Spatial Relative Distance
        double lowBound = SRD * alpha + (1.0 - alpha) * (double)minPED / (double)tau;
        if (canLabels[rowNum].labelScores.empty() || lowBound < canLabels[rowNum].labelScores.begin()->score) {
            int PED = 0;
            for (int i = 1; i <= keywordNum; i++) {
                int tempPED = 999;
                for (int j = 0; j < vertexNode[tempLabel.x].size(); j++) {
                    tempPED = min(tempPED, PEDList[i][vertexNode[tempLabel.x][j]]);
                }
                PED += tempPED;
            }
            if (PED < 999) {
                double PRD = (double) PED / (double) (tau * keywordNum); // Prefix Relative Distance
                labelScore ls(tempLabel.x, SRD, PED, SRD * alpha + (1.0 - alpha) * PRD);
                ls.source.push_back(rowNum);
                canLabels[rowNum].labelScores.insert(ls);
            }

            // update the labelPosList
            int pos = it->pos;
            vector<int> nodeUpdate;
            nodeUpdate.reserve(topActiveNodes.size());
            while (it != canLabels[rowNum].labelPosList.end()) {
                if (it->pos == pos) {
                    nodeUpdate.push_back(it->node);
                    it = canLabels[rowNum].labelPosList.erase(it);
                } else {
                    break;
                }
            }
            for (int i = 0; i < nodeUpdate.size(); i++) {
                auto sourceInfo = &canLabels[rowNum].sourceNode[nodeUpdate[i]];
                if (sourceInfo->qid >= sourceInfo->labelNum)
                    continue;
                int lp = sourceInfo->tag[sourceInfo->matchDepth].second;
//                tm.restart();
                for (int j = sourceInfo->matchDepth; j >= 0; j--) {
                    BitMap *bm = &nowPB->bitMaps[j][nowPB->nodeBitMapId[sourceInfo->matchPBNode][j]];
                    bm->nextOne(sourceInfo->tag[j].first, lp, sourceInfo->tag[j].second);
                }
//                cal_time += tm.getTimeMs();
                if (lp != -1)
                    canLabels[rowNum].labelPosList.insert(labelPos(lp, nodeUpdate[i]));
                sourceInfo->qid++;
            }
        } else {
            break;
        }
    }
    cal_time += tm.getTimeMs();
}

void addQ(int &num) {
    if (canLabels[num].labelScores.empty()) {
        return;
    }
    labelScore labelAdd = *canLabels[num].labelScores.begin(); // the label need to be enqueued
    canLabels[num].labelScores.erase(canLabels[num].labelScores.begin());

    // try to calculate the score after deleting the first label in labelScores
    CalScore(num);

    // determine if the vertex of label exists in the result set
    for (auto &it : result) {
        if (it.x == labelAdd.x) {
            addQ(num);
            return;
        }
    }

    // if the vertex of label exists in the queue
    for (auto &it : Q) {
        if (it.x == labelAdd.x) {
            labelScore tempLabel = it;
            tempLabel.source.push_back(num);
            tempLabel.score = min(tempLabel.score, labelAdd.score);
            tempLabel.SRD = min(tempLabel.SRD, labelAdd.SRD);
            Q.erase(it);
            Q.insert(tempLabel);
            return;
        }
    }

    // the vertex of label don't exist in the queue
    Q.insert(labelAdd);
}

void ExtendLabel() {
    if (Li >= queryLabelForward.size()) {
        return;
    }
    double frontDis = INT32_MAX;
    if (!Q.empty())
        frontDis = Q.begin()->score;
    while (Li < queryLabelForward.size()) {
        edgeS tempLabelForward = queryLabelForward[Li];
        auto extendPB = &vertexPB[tempLabelForward.x];
        if (extendPB->backwardList.empty()) {
            Li++;
            continue;
        }
        double lowBound = (double) (tempLabelForward.w + extendPB->backwardList[0].w) * alpha / dMax +
                          (double) minPED / (double) tau * (1.0 - alpha);
        if (lowBound >= frontDis)
            break;
        // get candidate labels from each active node
//        timer tm;
//        tm.restart();
        for (int i = 0; i < topActiveNodes.size(); i++) {
            TrieNode *activeNode = topActiveNodes[i];
            auto matchPBNode = extendPB->nodeSet.lower_bound(activeNode->id);
            if (matchPBNode != extendPB->nodeSet.end() && *matchPBNode <= activeNode->last) {
                auto *matchBitMapIds = &extendPB->nodeBitMapId[*matchPBNode];
                int matchDepth = min((int)matchBitMapIds->size() - 1, (int)activeNode->depth);
                BitMap *matchBitMap = &extendPB->bitMaps[matchDepth][(*matchBitMapIds)[matchDepth]];
                canLabels[Li].sourceNode[activeNode->id].labelNum = matchBitMap->countOne();
                canLabels[Li].sourceNode[activeNode->id].tag.resize(activeNode->depth + 1);
                canLabels[Li].sourceNode[activeNode->id].matchDepth = matchDepth;
                // init the tag of match node
                int lp = 0;  // label position
                for (int j = canLabels[Li].sourceNode[activeNode->id].matchDepth; j >= 0; j--) {
                    BitMap *bm = &extendPB->bitMaps[j][extendPB->nodeBitMapId[*matchPBNode][j]];
                    bm->nextOne(canLabels[Li].sourceNode[activeNode->id].tag[j].first, lp,
                                canLabels[Li].sourceNode[activeNode->id].tag[j].second);
                }
                canLabels[Li].sourceNode[activeNode->id].qid++;
                canLabels[Li].sourceNode[activeNode->id].matchPBNode = *matchPBNode;
                canLabels[Li].labelPosList.insert(labelPos(lp, activeNode->id));
            }
        }
//        extend_time += tm.getTimeMs();

        // take the label and calculate scores
        // score = alpha * SRD + (1-alpha) * PRD
        CalScore(Li);

        addQ(Li);
        Li++;
        if (!Q.empty())
            frontDis = Q.begin()->score;
    }
}

void Query1(vector<string> &resIds) {
    cout << "Query1\n";
    timer tm;
    tm.restart();
    mtrie = new MTrie(tau);
    mtrie->root->insertChild(trie->root, make_pair(0, 0));
    trash.clear();
    getActiveNods();
    if (topActiveNodes.empty()) {
        total_time += tm.getTimeMs();
        printf("There are no vertices that match the conditions!\n");
        return;
    }
    while (result.size() < k) {
        if (!Q.empty()) {
            auto front = Q.begin();
            labelScore frontLabel = *front;
            Q.erase(Q.begin());  // pop the first label
            result.insert(frontLabel);
            for (int i = 0; i < frontLabel.source.size(); i++) {
                addQ(frontLabel.source[i]);
            }
        } else if (Q.empty() && Li >= queryLabelForward.size()) {
            break;  // every possible label has been tried
        }
        ExtendLabel();
    }
    double time = tm.getTimeMs();

    for (auto i = result.begin(); i != result.end(); i++) {
        cout << i->x << " " << i->score << endl;
        resIds.push_back(to_string(i->x)+"-"+to_string(i->score));
    }
//    if (inputNum == 5) {
    total_strLen++;
    total_time += time;
//    }
    cout << "Query time is: " << time << endl;
}

void calScore2() {
    // item: node id
    // item1: 2-hop labels of the node
    // item2: the match label
    timer tm1;
    for (auto item = candidateLabel.begin(); item != candidateLabel.end(); item++) {
        if (result.size() == k) {
            auto it = result.end();
            --it;
            if ((double)item->w > it->score)
                break;
        }
//        // Determine if the vertices still exist
//        if (inputNum > tau) {
//            bool flag = false;
//            while (item != candidateLabel.end()) {
//                for (auto &item1: keyIds[keywordNum]) {
//                    if (std::binary_search(keyVertex[item1].begin(), keyVertex[item1].end(), item->x)) {
//                        flag = true;
//                        break;
//                    }
//                }
//                if (flag) {
//                    break;
//                } else {
//                    item = candidateLabel.erase(item);
//                }
//            }
//            if (item == candidateLabel.end()) {
//                break;
//            }
//        }
        tm1.restart();
        labelScore tempLabel;
        tempLabel.x = item->x;
        int totalPED = 0;
        for (int i = 1; i <= keywordNum; i++) {
            int PED = 999;
            for (auto &item1 : vertexNode[item->x]) {
                PED = min(PED, PEDList[i][item1]);
            }
            totalPED += PED;
        }
        cal_time += tm1.getTimeMs();
        if (totalPED < 999) {
            tempLabel.score =
                    alpha * (double) item->w / dMax + (1.0 - alpha) * (double) totalPED / (double) (keywordNum * tau);
            result.insert(tempLabel);
            if (result.size() > k) {
                result.erase(--result.end());
            }
        }
    }
}

void Query2(vector<string> &resIds) {
    cout << "Query2\n";
    timer tm;
    tm.restart();
    mtrie = new MTrie(tau);
    mtrie->root->insertChild(trie->root, make_pair(0, 0));
    getActiveNods();
    if (topActiveNodes.empty()) {
        printf("There are no vertices that match the conditions!\n");
        return;
    }
    calScore2();
    double time = tm.getTimeMs();
    total_time += time;
    total_strLen++;
    int count = 0;
    for (auto &item : result) {
        cout << item.x << " " << item.score << endl;
        resIds.push_back(to_string(item.x)+"-"+to_string(item.score));
        count++;
    }
    cout<< "Query time is: "<<time<<"ms"<<endl<<endl;
}

void QueryTest(vector<string> &resIds) {
    // cout << "QueryTest Parameter:"
    //      << "\nk = " << k
    //      << "\ntau = " << tau
    //      << "\nalpha = " << alpha
    //      << "\nqueryVertex = " << queryVertex
    //      << "\ndMax = " << dMax << endl;
    // query test
    // tau = 2;
    // k = 32;
    // dMax = 10000;
    // int queryStringNum = 2;
    srand((unsigned) time(NULL));
    total_strLen = 0;
//    int testNum = testCase.size();
    int testNum = 1;
    for (int i = 0; i < testNum; i++) {
        topActiveNodes.clear();  // active nodes at the first level
        activeDepth.clear();  // the depth of active node
        activeUp.clear();  // the Up if bound of active node
        trash.clear();
        PEDList.clear();
        minPEDs.clear();
        keyIds.clear();
        vertexes.clear();
        candidateLabel.clear();

        queryLabelForward.clear();
        Q.clear();  // priority queue
        result.clear();
        canLabels.clear();
        Li = 0;  // the number of vertex that should be extended in the backward list
        keywordNum = 0;

        // keywords
        // input.clear();
        // input.push_back("x");
        // for (int j = 0; j < queryStringNum; j++) {
        //     string tempKey = "";
        //     tempKey += "x";
        //     tempKey += recs[rand() % recs.size()];
        //     tempKey = tempKey.substr(0, rand() % tempKey.size() + 2);
        //     input.push_back(tempKey);
        //     cout << "tempKey = " << tempKey << endl;
        // }

        PEDList.resize(input.size());
        keyIds.resize(input.size());

        // process the keyword before the last one
        string inputString = "";
        for (int j = 1; j < input.size() - 1; j++) {
            inputNum = input[j].size() - 1;
            keywordNum++;
            inputString += input[j].substr(1, input[j].size());
            inputString += " ";
            // init compact tree
            mtrie = new MTrie(tau);
            mtrie->root->insertChild(trie->root, make_pair(0,0));

            getActiveNods();
            minPEDs.push_back(minPED);
        }

        // queryVertex = rand() % vertexNum;
        queryLabelForward = labelForward[queryVertex];

        // process the last keyword
        keywordNum++;
        inputNum = 0;
        for (int j = 0; j < input[keywordNum].length() - 1; j++) {
            canLabels.clear();
            canLabels.resize(queryLabelForward.size());
            Li = 0;
            result.clear();
            Q.clear();
            inputNum++;
            inputString += input[keywordNum][inputNum];
        }
        printf("query point：v%d\ninput:%s\ntop-%d:\n", queryVertex, inputString.c_str(), k);
        if (queryStringNum == 1) {
            Query1(resIds);
        } else {
            // get the vertexes corresponding to the first keyword
            for (auto &it : keyIds[1]) {
                vertexes.insert(keyVertex[it].begin(), keyVertex[it].end());
            }
            for (int j = 2; j < input.size() - 1; j++) {
                set<int> tempVertexes;
                for (auto &it : keyIds[j]) {
                    tempVertexes.insert(keyVertex[it].begin(), keyVertex[it].end());
                }
                set_intersection(vertexes.begin(), vertexes.end(), tempVertexes.begin(), tempVertexes.end(), inserter(vertexes, vertexes.begin()));
            }
            if (vertexes.size() >= 10000) {
                Query1(resIds);
                return;
            }

            std::fill(queryPointLabel.begin(), queryPointLabel.end(),-1);
            for (auto &item : labelForward[queryVertex]) {
                queryPointLabel[item.x] = item.w;
            }

            // calculate the distance
            for (auto &item: vertexes) {
                int dis = INT32_MAX;
                for (auto &item1 : labelForward[item]) {
                    if (queryPointLabel[item1.x] != -1) {
                        int tempDis = queryPointLabel[item1.x] + item1.w;
                        if (tempDis < dis)
                            dis = tempDis;
                    }
                }
                candidateLabel.insert(label(item, dis));
            }
            Query2(resIds);
        }
    }
};

// *********** find path ****************************
class labelP {
// public:
public:
    int vid;
    int dis;
    int pre;
    labelP() {
        vid = 0;
        dis = 0;
        pre = 0;
    }
    labelP(int vid, int dis, int pre) {
        this->vid = vid;
        this->dis = dis;
        this->pre = pre;
    }
    bool operator<(const labelP &label1) const {
        if (label1.dis != this->dis)
            return label1.dis > this->dis;
        else
            return label1.vid < this->vid;
    }
};

vector<vector<labelP>> labelForwardPath;
vector<int> path;
// int vertexNum;
int startVertex, endVertex;

void LoadPath() {
    ifstream inFile(pathName, ios::binary | ios::out);
    inFile.read((char *)(&vertexNum), sizeof(vertexNum));
    // cout << "vertexNum in LoadPath = " << vertexNum << endl;
    labelForwardPath.resize(vertexNum);
    for (int i = 0; i < vertexNum; i++) {
        if (i % 10000 == 0) {
            printf("%d/%d\n", i, vertexNum);
        }
        int labelNum;
        inFile.read((char *)(&labelNum), sizeof(labelNum));
        // cout << "\t" << labelNum << endl;
        for (int j = 0; j < labelNum; j++) {
            // cout << "\t\t" << j << endl;
            int vid, dis, pre;
            inFile.read((char *)(&vid), sizeof(vid));
            inFile.read((char *)(&dis), sizeof(dis));
            inFile.read((char *)(&pre), sizeof(pre));
            labelForwardPath[i].push_back(labelP(vid, dis, pre));
        }
    }
    inFile.close();
}

void findPath(int startVertex, int endVertex) {
    // cout << "findPath " << startVertex << " " << endVertex << endl;
    if (startVertex == endVertex)
        return;
    path.push_back(startVertex);
    int dis = INT32_MAX;
    int pre1, pre2, mid;
    for (auto &item : labelForwardPath[startVertex]) {
        for (auto &item1: labelForwardPath[endVertex]) {
            if (item.vid == item1.vid && (item.dis + item1.dis) < dis) {
                dis = item.dis + item1.dis;
                pre1 = item.pre;
                pre2 = item1.pre;
                mid = item.vid;
            }
        }
    }
    if (pre1 != endVertex)
        findPath(startVertex, pre1);
    if (pre1 != startVertex || mid != endVertex)
        findPath(pre1, mid);

    if (mid != startVertex && mid != endVertex)
        path.push_back(mid);

    if (mid != startVertex || pre2 != endVertex)
        findPath(mid, pre2);
    if (pre2 != startVertex)
        findPath(pre2, endVertex);
    path.push_back(endVertex);
}

void QueryPath(vector<string> &resIds) {
    srand((int)time(0));
    path.clear();
    // int startVertex = rand() % vertexNum;
    // int endVertex = rand() % vertexNum;
    // cout << startVertex << " " << endVertex << endl;
    findPath(startVertex, endVertex);
    for (auto item = path.begin(); item != path.end();) {
        auto item1 = item + 1;
        if (item1 != path.end() && *item1 == *item) {
            item = path.erase(item);
        } else {
            item++;
        }
    }
    // cout << "path: ";
    for (auto item : path) {
        resIds.push_back(to_string(item));
        // cout << item << " ";
    }
    // cout << endl;
}

int main(int argc, char **argv) {
    char recvbuf[BUFFER_SIZE];
    string cityName = "NY";
    memset(recvbuf, 0, sizeof(recvbuf));

    srand((unsigned) time(NULL));

    string filePath = "../datasets/" + cityName + "/" + cityName;
    txtName = (char*) malloc(1+filePath.length() + 4);
    strcpy(txtName,filePath.c_str());
    outName = "../datasets/" + cityName + "/" + cityName + ".out";

    // get vertexNum
    ifstream inFile;
    inFile.open(filePath);
    inFile >> vertexNum;
    inFile.close();

    // rebuild trie
    string stringFileName = "../datasets/" + cityName + "/" + cityName + ".string";
    readData(stringFileName, recs);
    testFileName = "../datasets/" + cityName + "/" + cityName + ".test";

    // construct trie index
    printf("Starting to build trie index...\n");
    trie = new Trie();
    for (auto i = 0; i < recs.size(); i++)
        trie->append(recs[i].c_str(), i);
    trie->buildIdx();
    trie->bfsIndex();
    PEDList.resize(trie->root->last+1);
    printf("Trie index building complete.\n");

    // Load index
    printf("Loading index...\n");
    LoadLabelForward();
    LoadPB();

    // Load path
    printf("Loading path...\n");
    pathName = "../datasets/" + cityName + "/"  + cityName + ".path";
    LoadPath();

    printf("Start querying...\n\n");
    // init
    LoadVertexNode();
    malloc_trim(0);
    queryPointLabel.resize(vertexNum);
    std::fill(queryPointLabel.begin(), queryPointLabel.end(),-1);

    double cost_time;

    vector<string> resIds;
    while (true) {
        int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(MYPORT);  
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    
        // waiting for connect
        while (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        {
            // cout << "TASK connection error\n";
            // exit(1);
        }

        char recvbuf[BUFFER_SIZE];
        cout << "Waiting for the query location...\n";
        // while(true) {
        //     recv(sock_cli, recvbuf, sizeof(recvbuf),0);
        //     if(int(recvbuf[0]) != 0) break;
        // }
        recv(sock_cli, recvbuf, sizeof(recvbuf),0);
        if(int(recvbuf[0]) == 0) continue;

        int queryType;
        if(recvbuf[0]=='x') queryType=1;        // queryPoints
        else if(recvbuf[0]=='p') queryType=2;   // queryPath
        cout << "queryType = " << queryType << endl;

        string infoHead = "";
        int idx = 1;
        for(; idx<BUFFER_SIZE; ++idx) {
            if(recvbuf[idx] == ',') {
                ++idx;
                break;
            }
            infoHead += recvbuf[idx];
        }

        int infoLength = stoi(infoHead.c_str());
        cout << "infoLength = " << infoLength << endl;

        string info = recvbuf;
        info = info.substr(idx, infoLength);
        memset(recvbuf, 0, sizeof(recvbuf));
        cout << "info = " << info << endl;

        vector<string> queryVertex_inputs;
        vector<string> temp_vec;
        splitInfo(queryVertex_inputs, info, delimiter);

        int info_item_num = queryVertex_inputs.size();
        // if(info_item_num%2 != ) {
        //     cout << "Wrong number of data.\n";
        //     continue;
        // }
        cout << "queryVertex_inputs : " << endl;
        for(auto item : queryVertex_inputs) {
            cout << item << "  ";
        }
        cout << endl;
        
        resIds.clear();

        if(queryType == 1) {
            cout << "---------query points---------\n";
            // cin >> queryVertex;
            k = stoi(queryVertex_inputs[info_item_num-5].c_str());
            tau = stoi(queryVertex_inputs[info_item_num-4].c_str());
            alpha = stod(queryVertex_inputs[info_item_num-3].c_str());

            queryVertex = stoi(queryVertex_inputs[info_item_num-2].c_str());
            if (queryVertex == -1)
                break;
            // cin >> input;
            string inputInfo = queryVertex_inputs[info_item_num-1];
            kws.clear();
            splitInfo(kws, inputInfo, space);

            input.clear();
            input.push_back("x");
            for(auto &kw : kws) {
                input.push_back("x"+kw);
            }
            queryStringNum = kws.size();
            timer tm;
            tm.restart();
            QueryTest(resIds);
            cost_time = tm.getTimeMs();
        } else if(queryType == 2) {
            cout << "---------query path---------\n";
            startVertex = stoi(queryVertex_inputs[info_item_num-2].c_str());
            endVertex = stoi(queryVertex_inputs[info_item_num-1].c_str());
            timer tm;
            tm.restart();
            QueryPath(resIds);
            cost_time = tm.getTimeMs();
            temp_vec.clear();
            float step = resIds.size()*1.0/18, cnt=0;
            for(int i=int(cnt); i<resIds.size()-1; ) {
                temp_vec.push_back(resIds[i]);
                cnt += step;
                i = int(cnt);
            }
            temp_vec.push_back(resIds[resIds.size()-1]);
            resIds.swap(temp_vec);
        }
        // cout << "resIds:\n";
        // for(auto rid : resIds) {
        //     cout << "  " << rid;
        // }
        // cout << endl;
        
        string resInfo = "";

        if(resIds.size() == 0) {
            resInfo = "NORESULT";
        } else {
            resInfo = resIds[0];
            for(int i=1; i<resIds.size(); ++i) {
                resInfo += ("," + resIds[i]);
            }
            resInfo += ("+"+to_string(cost_time/20));
        }

        const char *sendbuf = resInfo.c_str();
        cout << "sendbuf = " << sendbuf << endl;
        int send_error = send(sock_cli, sendbuf, strlen(sendbuf), MSG_NOSIGNAL);
        cout << "send_error = " << send_error << endl;
        
        close(sock_cli);
    }

    // cout << "total" << total_time / (double)total_strLen << endl;
    // cout << "extend" << extend_time / (double)total_strLen << endl;
    // cout << "cal" << cal_time / (double)total_strLen << endl;
    return 0;
}
