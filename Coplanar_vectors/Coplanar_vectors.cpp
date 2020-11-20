#include <iostream>
#include <iomanip>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

int num = 0; //number of coplanar sets

const int threadNumber = 4; // thread quantity

//defines if three current vectors are coplanar
bool AreCoplanar(vector<int> v1, vector<int> v2, vector<int> v3)
{
    double res = v2[2] * v1[1] * v3[0] + v1[0] * v2[1] * v3[2] + v1[2] * v2[0] * v3[1]
        - v1[2] * v2[1] * v3[0] - v2[0] * v1[1] * v3[2] - v1[0] * v2[2] * v3[1];
    return res == 0;
}

//main thread func to find coplanar vectors
void thread_coplfunc(int iTread, int iTN, vector<vector<int>> arr, int size) {
    for (int i = iTread; i < size - 2; i += iTN) {
        for (int j = i + 1; j < size - 1; j++)
            for (int k = j + 1; k < size; k++)
                if (AreCoplanar(arr[i], arr[j], arr[k])) num++;
    }
}

//transforms string vector coords into integer vector coords
vector<int> TransformToVect(string line)
{
    vector<int> res;
    res.reserve(3);
    for (int i = 0; i < 2; i++) {
        res.push_back(stoi(line.substr(0, line.find(',') + 1)));
        line = line.erase(0, line.find(',') + 1);
    }
    res.push_back(stoi(line));
    return res;
}

//transforms string vector array into vector array
vector<vector<int>> GetVectorArray(string* data, int arr_len)
{
    vector<vector<int>> vectors;
    vectors.resize(arr_len);
    for (int i = 0; i < arr_len; i++)
    {
        vectors[i] = TransformToVect(data[i]);
    }
    return vectors;
}

//reads vectors from file
class Reader
{
private:
    string pathToInput;
    string* output_arr;
public:
    int quan;
    Reader(string path)
    {
        pathToInput = path;
    }

    string* GetVectors()
    {
        ifstream fs(pathToInput);
        if (!(fs.is_open())) {
            throw "Test data file failed to open.";
        }
        string num;
        getline(fs, num);
        quan = stoi(num);
        output_arr = new string[quan];
        for (int i = 0; getline(fs, num); i++) {
            output_arr[i] = num;
        }
        fs.close();
        return output_arr;
    }
};

//counts coplanar vector number
void CoplanNumber(vector<vector<int>> arr, int arrSize)
{
    std::thread* thr[threadNumber];
    double sum[threadNumber];
    for (int i = 0; i < threadNumber; i++)
        sum[i] = 0;

    // Thread creating
    for (int i = 0; i < threadNumber; i++) {
        thr[i] = new std::thread{ thread_coplfunc, i, threadNumber, arr, arrSize };
    }

    // Thread ending
    for (int i = 0; i < threadNumber; i++) {
        thr[i]->join();
        delete thr[i];
    }
}

int main() {
	//all the files with vectors should be put into the "input" folder
	//you should write only the name of the file, not its path
    cout << "Enter filename: ";
    string path;
    cin >> path;
    path = "input/" + path;
    try {
        Reader reader = Reader(path);
        string* lines = reader.GetVectors();
        vector<vector<int>> vectorArr = GetVectorArray(lines, reader.quan);
        CoplanNumber(vectorArr, reader.quan);
    }
	catch (exception)
	{
        cout << "Error!";
        return 1;
	}
    cout << num << endl;
    return 0;
}
