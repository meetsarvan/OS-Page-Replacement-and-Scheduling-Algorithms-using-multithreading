#include <bits/stdc++.h>
#include <thread>
#include <mutex>
using namespace std;

const int MISS = 0;
const int TOTAL = 1;

// Declaration of Classes
class history;
class handler;
class input;
class output;
class analyze;
class process;
class RAM;
class algoData;
class FIFO;
class LRU;
class MRU;
class OPT;

// Definition of Classes
class history
{
private:
    static history *currentInstance;
    vector<pair<input *, output *>> hist;

public:
    static history *getInstance();
    history(){};
    void updateHistory(input *in, output *out);
    pair<input *, output *> getLastElement();
    void printCurrentStats();
};

class handler
{
    int RAMSize;
    int noOfProcess;
    int processSize;

    handler(int RAMSize, int noOfProcess, int processSize);

public:
    static handler *createHandler();
    void analyzeOnAllPageSize();
    void printAnalyzedData();
};

class analyze
{
    int noOfProcess;
    int noOfPages;
    int noOfRAMPages;
    vector<vector<int>> curOutput;

    analyze(int noOfProcess, int RAMSize, int processSize);

public:
    static analyze *createAnalyze(int noOfProcess, int RAMSize, int processSize, int pageSize);
    void runProcesses();

private:
    void mergeOutput(vector<vector<int>> curOutput);
};

class process
{
    int noOfRAMPages;
    int noOfPages;
    vector<int> pageID; // randomly generated, with length predefined

    process(int noOfPages, int noOfRAMPages, vector<int> pageID);

public:
    static process *createProcess(int noOfPages, int noOfRAMPages);
    vector<vector<int>> runProcess();
};

class RAM
{
    int noOfRAMPages;
    int noOfPages;
    vector<int> pageID;

public:
    RAM(int noOfRAMPages, int noOfPages, vector<int> pageID);
    virtual vector<int> processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID) = 0;
};

class algoData
{
public:
    algoData(function<RAM *(int, int, vector<int>)> createFunction, int algoID);
    function<RAM *(int, int, vector<int>)> createFunction;
    int algoID;
};

// input
class input
{
    int RAMSize;
    int noOfProcess;
    int processSize;
    input(int RAMSize, int noOfProcess, int processSize);

private:
    static input *getInput();

public:
    static void createHistory();
    int getRAMSize();
    int getNoOfProcess();
    int getProcessSize();
};

class output
{
    output();

public:
    vector<vector<vector<int>>> mainOutput;
    void mergeOutput(vector<vector<int>> curOutput);
    static output *getOutput();
};

class FIFO : public RAM
{
public:
    FIFO(int noOfRAMPages, int noOfPages, vector<int> pageID) : RAM(noOfRAMPages, noOfPages, pageID){};
    vector<int> processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID) override;
};

class LRU : public RAM
{
public:
    LRU(int noOfRAMPages, int noOfPages, vector<int> pageID) : RAM(noOfRAMPages, noOfPages, pageID){};
    vector<int> processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID) override;
};

class MRU : public RAM
{
public:
    MRU(int noOfRAMPages, int noOfPages, vector<int> pageID) : RAM(noOfRAMPages, noOfPages, pageID){};
    vector<int> processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID) override;
};

class OPT : public RAM
{
public:
    OPT(int noOfRAMPages, int noOfPages, vector<int> pageID) : RAM(noOfRAMPages, noOfPages, pageID){};
    vector<int> processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID) override;
};

// Mapping of algo to its name
unordered_map<string, algoData *> mapping = {
    {"OPT",  new algoData([](int noOfRAMPages,int noOfPages,vector<int> pageID){
                   return new OPT(noOfRAMPages,noOfPages,pageID);
               }, 1)},
    {"FIFO", new algoData([](int noOfRAMPages,int noOfPages,vector<int> pageID){
                   return new FIFO(noOfRAMPages,noOfPages,pageID);
               }, 2)},
    {"LRU",  new algoData([](int noOfRAMPages,int noOfPages,vector<int> pageID){
                   return new LRU(noOfRAMPages,noOfPages,pageID);
               }, 3)},
    {"MRU",  new algoData([](int noOfRAMPages,int noOfPages,vector<int> pageID){
                   return new MRU(noOfRAMPages,noOfPages,pageID);
               }, 4)}};

// Definition of Functions

// history class
history *history::currentInstance = NULL;

void history::printCurrentStats()
{
    input *currInput = hist.back().first;
    output *currOutput = hist.back().second;

    int noOfRows = currOutput->mainOutput.size();
    int noOfColumns = mapping.size()+1;
    vector<vector<string>> table(noOfRows,vector<string>(noOfColumns));
    cout << "Results:" << endl;
    table[0][0] = "Page Size";
    for(int i=1;i<noOfRows;i++){
        table[i][0] = to_string(i);
    }
    for(auto it:mapping){
        table[0][it.second->algoID] = it.first+"(Hit Rate)";
        for (int i = 1; i < noOfRows; i++)
        {
            int total = currOutput->mainOutput[i][it.second->algoID][TOTAL];
            int miss = currOutput->mainOutput[i][it.second->algoID][MISS];
            table[i][it.second->algoID] = to_string(1.0 * (total - miss) / total);
        }
    }
    for(int j=0;j<noOfColumns;j++){
        int mxLen = 0;
        for(int i=0;i<noOfRows;i++){
            mxLen = max(mxLen,(int)table[i][j].size());
        }
        for(int i=0;i<noOfRows;i++){
            while(((int)table[i][j].size()) != mxLen){
                table[i][j] += ' ';
            }
        }
    }

    vector<string> rowString(noOfRows+2);
    for(int i=0;i<noOfRows;i++){
        rowString[i+1] += "| ";
        for(int j=0;j<noOfColumns;j++){
            rowString[i+1] += table[i][j];
            rowString[i+1] += " | ";
        }
    }
    while(rowString[0].size() != rowString[1].size()){
        rowString[0] += "-";
        rowString[noOfRows+1] += "-";
    }

    for(auto it: rowString){
        cout<<it<<endl;
    }
}

history *history::getInstance()
{
    if (currentInstance == NULL)
    {
        currentInstance = new history();
        return currentInstance;
    }
    else
    {
        return currentInstance;
    }
}

void history::updateHistory(input *in, output *out)
{
    hist.push_back({in, out});
}

pair<input *, output *> history::getLastElement()
{
    return hist.back();
}

// output class
output::output() {}

output *output::getOutput()
{
    return new output();
}

void output::mergeOutput(vector<vector<int>> curOutput)
{
    this->mainOutput.push_back(curOutput);
}

// input class
input::input(int RAMSize, int noOfProcess, int processSize)
{
    this->RAMSize = RAMSize;
    this->noOfProcess = noOfProcess;
    this->processSize = processSize;
}

input *input::getInput()
{
    int RAMSize;
    int noOfProcess;
    int processSize;
    cout << "Enter the number of processes on which you want to test the compatability of different algorithms" << endl;
    cin >> noOfProcess;
    cout << "Enter the RAM size for which you want to test the compatability of different algorithms" << endl;
    cin >> RAMSize;
    cout << "Enter the size of the process on which you want to test the compatability of different algorithms" << endl;
    cin >> processSize;
    return new input(RAMSize, noOfProcess, processSize);
}

void input::createHistory()
{
    input *in = input::getInput();
    output *out = output::getOutput();
    history *instance = history::getInstance();
    instance->updateHistory(in, out);
}

int input::getRAMSize()
{
    return this->RAMSize;
}

int input::getNoOfProcess()
{
    return this->noOfProcess;
}

int input::getProcessSize()
{
    return this->processSize;
}

// Handler Class
handler::handler(int RAMSize, int noOfProcess, int processSize)
{
    this->RAMSize = RAMSize;
    this->noOfProcess = noOfProcess;
    this->processSize = processSize;
}

handler *handler::createHandler()
{
    history *instance = history::getInstance();
    input::createHistory();
    input *curInput = instance->getLastElement().first;
    return new handler(curInput->getRAMSize(), curInput->getNoOfProcess(), curInput->getProcessSize());
}

void handler::analyzeOnAllPageSize()
{
    for (int curPageSize = 0; curPageSize <= min(RAMSize,processSize); curPageSize++)
    {
        analyze *curAnalyze = analyze::createAnalyze(noOfProcess, RAMSize, processSize, curPageSize);
        curAnalyze->runProcesses();
    }
}

void handler::printAnalyzedData()
{
    history *instance = history::getInstance();
    instance->printCurrentStats();
}

// Analyze Class
analyze::analyze(int noOfProcess, int noOfPages, int noOfRAMPages)
{
    this->noOfProcess = noOfProcess;
    this->noOfPages = noOfPages;
    this->noOfRAMPages = noOfRAMPages;
}

analyze *analyze::createAnalyze(int noOfProcess, int RAMSize, int processSize, int pageSize)
{
    int noOfPages = (pageSize == 0 ? -1 : ((processSize + pageSize - 1) / pageSize));
    int noOfRAMPages = (pageSize == 0 ? -1 : (RAMSize / pageSize));
    return new analyze(noOfProcess, noOfPages, noOfRAMPages);
}

void analyze::runProcesses()
{
    for (int i = 0; i < noOfProcess; i++)
    {
        process *curProcess = process::createProcess(noOfPages, noOfRAMPages);
        vector<vector<int>> curOutput = curProcess->runProcess();
        mergeOutput(curOutput);
    }
    history *instance = history::getInstance();
    output *mainOutput = instance->getLastElement().second;
    mainOutput->mergeOutput(this->curOutput);
}

void analyze::mergeOutput(vector<vector<int>> curOutput)
{
    for (int i = 0; i < curOutput.size(); i++)
    {
        if (this->curOutput.size() > i)
        {
            this->curOutput[i][TOTAL] += curOutput[i][TOTAL];
            this->curOutput[i][MISS] += curOutput[i][MISS];
        }
        else
        {
            this->curOutput.push_back(curOutput[i]);
        }
    }
}

// Process class
process::process(int noOfPages, int noOfRAMPages, vector<int> pageID)
{
    this->noOfPages = noOfPages;
    this->noOfRAMPages = noOfRAMPages;
    this->pageID = pageID;
}

process *process::createProcess(int noOfPages, int noOfRAMPages)
{
    vector<int> pageID;
    int noOfBlocks = 100 * noOfPages;
    for (int i = 0; i < noOfBlocks; i++)
    {
        int pid = (rand() % noOfPages) + 1;
        pageID.push_back(pid);
    }
    return new process(noOfPages, noOfRAMPages, pageID);
}

vector<vector<int>> process::runProcess()
{
    // Added 1 element to make it 1-based indexed
    vector<vector<int>> processOutput(mapping.size()+1, vector<int>(2, -1));
    if (noOfPages == -1) {
        for (auto it : mapping) {
            processOutput[it.second->algoID] = {-1, -1};
        }
        return processOutput;
    }

    std::mutex mtx;
    vector<std::thread> threads;
    threads.reserve(mapping.size());

    for (auto it : mapping) {
        threads.emplace_back([&processOutput, &mtx, it, this]() {
            RAM *proc = it.second->createFunction(noOfPages, noOfRAMPages, pageID);
            vector<int> result = proc->processRAM(noOfPages, noOfRAMPages, pageID);
            {
                std::lock_guard<std::mutex> lock(mtx);
                processOutput[it.second->algoID] = result;
            }
            delete proc;
        });
    }

    for (auto &t : threads) {
        if (t.joinable()) t.join();
    }

    return processOutput;
}

// RAM class
RAM::RAM(int noOfRAMPages, int noOfPages, vector<int> pageID)
{
    this->noOfPages = noOfPages;
    this->noOfRAMPages = noOfRAMPages;
    this->pageID = pageID;
}

// algoData class
algoData::algoData(function<RAM *(int, int, vector<int>)> createFunction, int algoID)
{
    this->algoID = algoID;
    this->createFunction = createFunction;
}

// FIFO class
vector<int> FIFO::processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID)
{
    int missCount = 0;
    int total = pageID.size();
    unordered_set<int> chachedPages;
    queue<int> inOrder;
    for (int i = 0; i < pageID.size(); i++)
    {
        if (!chachedPages.count(pageID[i]))
        {
            missCount++;
            if (chachedPages.size() == noOfRAMPages){
                chachedPages.erase(inOrder.front());
                inOrder.pop();
            }
            chachedPages.insert(pageID[i]);
            inOrder.push(pageID[i]);
        }
    }
    return {missCount, total};
}

// LRU class
vector<int> LRU::processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID)
{
    int missCount = 0;
    int total = pageID.size();

    unordered_set<int> s;
    unordered_map<int, int> indexes;

    for (int i = 0; i < pageID.size(); i++)
    {
        if (s.size() < noOfRAMPages)
        {
            if (s.find(pageID[i]) == s.end())
            {
                s.insert(pageID[i]);
                missCount++;
            }
            indexes[pageID[i]] = i;
        }
        else
        {
            if (s.find(pageID[i]) == s.end())
            {
                int lru = INT_MAX, val;
                for (auto it = s.begin(); it != s.end(); it++)
                {
                    if (indexes[*it] < lru)
                    {
                        lru = indexes[*it];
                        val = *it;
                    }
                }

                s.erase(val);
                s.insert(pageID[i]);
                missCount++;
            }
            indexes[pageID[i]] = i;
        }
    }
    return {missCount, total};
}

// MRU class
vector<int> MRU::processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID)
{
    int missCount = 0;
    int total = pageID.size();

    unordered_set<int> s;
    unordered_map<int, int> indexes;

    for (int i = 0; i < pageID.size(); i++)
    {
        if (s.size() < noOfRAMPages)
        {
            if (s.find(pageID[i]) == s.end())
            {
                s.insert(pageID[i]);
                missCount++;
            }
            indexes[pageID[i]] = i;
        }
        else
        {
            if (s.find(pageID[i]) == s.end())
            {
                int mru = -1, val;
                for (auto it = s.begin(); it != s.end(); it++)
                {
                    if (indexes[*it] > mru)
                    {
                        mru = indexes[*it];
                        val = *it;
                    }
                }

                s.erase(val);
                s.insert(pageID[i]);
                missCount++;
            }
            indexes[pageID[i]] = i;
        }
    }
    return {missCount, total};
}

// OPT class
vector<int> OPT::processRAM(int noOfPages, int noOfRAMPages, vector<int> pageID)
{
    int missCount = 0;
    int total = pageID.size();

    vector<int> nxtOcc(total,total);
    map<int,int> nearestOcc;
    for(int i=total-1;i>=0;i--){
        if(nearestOcc.find(pageID[i]) != nearestOcc.end()){
            nxtOcc[i] = nearestOcc[pageID[i]];
        }
        nearestOcc[pageID[i]] = i;
    }

    unordered_set<int> chachedPages;
    set<int> nxtOccOfChachedPages;

    for (int i = 0; i < pageID.size(); i++)
    {
        if(!chachedPages.count(pageID[i])){
            if(chachedPages.size() == noOfRAMPages){
                int pageToRemove = pageID[*nxtOccOfChachedPages.rbegin()];
                nxtOccOfChachedPages.erase(*nxtOccOfChachedPages.rbegin());
                chachedPages.erase(pageToRemove);
            }
            chachedPages.insert(pageID[i]);
            nxtOccOfChachedPages.insert(nxtOcc[i]);
            missCount++;
        }
        else{
            nxtOccOfChachedPages.erase(i);
            nxtOccOfChachedPages.insert(nxtOcc[i]);
        }
    }
    return {missCount, total};
}

int main()
{
    handler *run = handler::createHandler();
    run->analyzeOnAllPageSize();
    run->printAnalyzedData();
    return 0;
}
