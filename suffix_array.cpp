#include <iostream>

using namespace std;

class SuffixArray {
private:
    //string and its len
    char* fullString;
    int len;

    //suffix array, ranks of every suffix in the suff arr, temp SA for sorting
    int* suffixArr;
    int* Suffixranks;
    int* tmpSuffixranks;   
    

public:
    SuffixArray(const char* input) {

        int length = 0;
        while (input[length] != '\0') {
            length++;
        }
        len = length;

        fullString = new char[len + 1];
        int i = 0;
        while (input[i] != '\0') {
            fullString[i] = input[i];
            i++;
        }
        fullString[i] = '\0';

        suffixArr = new int[len];
        Suffixranks = new int[len];
        tmpSuffixranks = new int[len];
    }

    void ConstructUsingPrefixDoubling() { 

        //initializ SA to each char index and SA ranks to its ascii char just for first sorting
        for (int i = 0; i < len; i++) {
            suffixArr[i] = i;
            Suffixranks[i] = fullString[i];
        }

        //O(logn)
        for (int offSet = 1; offSet < len; offSet *= 2) {
            
            sortSuffixesByRankAtOffSet(offSet);

            //after sorting we update suffix ranks with which has smaller rank at offset
            tmpSuffixranks[suffixArr[0]] = 0;
            for (int i = 1; i < len; i++) {
                
                if (compare(suffixArr[i - 1], suffixArr[i], offSet)) {
                    tmpSuffixranks[suffixArr[i]] = tmpSuffixranks[suffixArr[i - 1]] + 1;

                } else {
                    tmpSuffixranks[suffixArr[i]] = tmpSuffixranks[suffixArr[i - 1]];

                }

            }

            //set SA ranks to the constructed tempSAranks
            for (int i = 0; i < len; ++i) {
                Suffixranks[i] = tmpSuffixranks[i];

            }
        }
    }

    void Print() const {
        for (int i = 0; i < len; i++) {
            cout << suffixArr[i] << " ";
        }
        cout << "\n";
    }

private:
    //compair returns which suffix has smaller suffix rank at distance offSet, -1 if over len
    bool compare(int i, int j, int offSet) const {
        if (Suffixranks[i] != Suffixranks[j]) {
            return Suffixranks[i] < Suffixranks[j];
        }
        int ranki = (i + offSet < len) ? Suffixranks[i + offSet] : -1;
        int rankj = (j + offSet < len) ? Suffixranks[j + offSet] : -1;
        return ranki < rankj;
        // if ranks are equal return false to not inc rank
    }

    //sort based on smaller = has smaller rank at offset   O(nlogn)
    void sortSuffixesByRankAtOffSet(int offSet) {
        
        int* tempSAforSorting = new int[len];
        mergeSort(0, len - 1, offSet, tempSAforSorting);

    }

    void mergeSort(int left, int right, int offSet, int* tempSAforSorting) {

        if (left >= right) {
            return;
        } 

        int mid = left + (right - left) / 2;

        mergeSort(left, mid, offSet, tempSAforSorting);
        mergeSort(mid + 1, right, offSet, tempSAforSorting);

        merge(left, mid, right, offSet, tempSAforSorting);
    }

    //merge halves and save again in the suffixArray
    void merge(int left, int mid, int right, int offSet, int* tempSAforSorting) {

        int i = left, j = mid + 1, idx = left;

        while (i <= mid && j <= right) {
            if (compare(suffixArr[i], suffixArr[j], offSet)) {
                tempSAforSorting[idx++] = suffixArr[i++];
            } else {
                tempSAforSorting[idx++] = suffixArr[j++];
            }
        }

        while (i <= mid) {
            tempSAforSorting[idx++] = suffixArr[i++];
        }

        while (j <= right) {
            tempSAforSorting[idx++] = suffixArr[j++];
        }

        for (int i = left; i <= right; ++i) {
            suffixArr[i] = tempSAforSorting[i];
        }
    }
};

int main() {
    SuffixArray t("ACGACTACGATAAC$");

    t.ConstructUsingPrefixDoubling();

    t.Print(); // Expected output: 14 11 12 0 6 3 9 13 1 7 4 2 8 10 5

    return 0;
}
