#include <iostream>

using namespace std;

class SuffixArray {
    //suffix array, ranks of every suffix in the suff arr, temp SA for sorting
    int* suffixArr;
    int* Suffixranks;
    int* tmpSuffixranks;

    //string and its len
    char* fullString;
    int len;

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

        //initializ ranks to ascii one time
        for (int i = 0; i < len; i++) {
            Suffixranks[i] = fullString[i];
            suffixArr[i] = i;
        }

    }

    //compair returns which suffix has smaller suffix rank at distance offSet, -1 if over len
    bool checksmallerSuffix(int si, int sj, int offSet) const {
        int ranki = 0; 
        int rankj = 0;
        if (Suffixranks[si] != Suffixranks[sj]) {
            return Suffixranks[si] < Suffixranks[sj];
        } else {

            if (si + offSet < len) {
                ranki = Suffixranks[si + offSet];
            } else { ranki = -1; }

            if (sj + offSet < len) {
                rankj = Suffixranks[sj + offSet];
            } else { rankj = -1; }

            return ranki < rankj;
            // if ranks are equal return true to not inc rank
        }
    }

    void mergeSort(int p, int q, int offSet, int* tempSAforSorting) {
        
        if (p >= q) {
            return;
        } 
        
        int r = p;
        r += (q - p) / 2;

        mergeSort(p, r, offSet, tempSAforSorting);
        mergeSort(r + 1, q, offSet, tempSAforSorting);

        merge(p, r, q, offSet, tempSAforSorting);
    }

    //merge halves and save again in the suffixArray
    void merge(int p, int r, int q, int offSet, int* tempSAforSorting) {

        int pptr = p;
        int qptr = r + 1;
        int tempi = p;

        while (pptr <= r && qptr <= q) {
            if (checksmallerSuffix(suffixArr[pptr], suffixArr[qptr], offSet)) {
                tempSAforSorting[tempi++] = suffixArr[pptr++];
            } else {
                tempSAforSorting[tempi++] = suffixArr[qptr++];
            }
        }
        while (pptr <= r) {
            tempSAforSorting[tempi] = suffixArr[pptr];
            tempi++;
            pptr++;
        }
        while (qptr <= q) {
            tempSAforSorting[tempi] = suffixArr[qptr];
            tempi++;
            qptr++;
        }

        for (int i = p; i <= q; ++i) {
            suffixArr[i] = tempSAforSorting[i];
        }
    }

    void ConstructUsingPrefixDoubling() { 

        //O(logn)
        for (int offSet = 1; offSet < len; offSet *= 2) {
            
            // sort suffixArr based on rankArr
            int* tempSAforSorting = new int[len];
            mergeSort(0, len - 1, offSet, tempSAforSorting);

            //after sorting we update suffix ranks with which has smaller rank at offset
            for (int i = 0; i < len; i++) {

                if(i == 0) {
                    tmpSuffixranks[suffixArr[0]] = 0;
                    continue;

                } else {
                    tmpSuffixranks[suffixArr[i]] = tmpSuffixranks[suffixArr[i-1]];
                    if (checksmallerSuffix(suffixArr[i-1], suffixArr[i], offSet)) {
                        tmpSuffixranks[suffixArr[i]]++;
                    }
                }
            }

            //set SA ranks to the constructed tempSAranks
            for (int i = 0; i < len; i++) {
                Suffixranks[i] = tmpSuffixranks[i];

            }
        }
    }

    void Print() const {
        for (int i = 0; i < len; i++) {
            cout << suffixArr[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    SuffixArray t("ACGACTACGATAAC$");

    t.ConstructUsingPrefixDoubling();

    t.Print(); // Expected output: 14 11 12 0 6 3 9 13 1 7 4 2 8 10 5

    return 0;
}
