#define LUMP_SIZE 16
#define BRUSHSIDE_SIZE 8
#define BRUSHSIDE_LUMP_INDEX 19

#include <iostream>
#include <fstream>
using namespace std;

// Get all texinfo and link them by flag
//

int main () {
    ifstream file("test.bsp", ios::binary);

    if (file.is_open()){

        // Ignore the first two variables of the BSP file header
        file.seekg(sizeof(int), ios::cur); // ident
        file.seekg(sizeof(int), ios::cur); // version

        file.seekg(LUMP_SIZE * BRUSHSIDE_LUMP_INDEX, ios::cur);

        int lumpOffset, lumpLen;
        file.read(reinterpret_cast<char*>(&lumpOffset), sizeof(int));
        file.read(reinterpret_cast<char*>(&lumpLen), sizeof(int));

        // Put the file data into a buffer
        file.seekg(0, ios::end);
        streampos size = file.tellg();
        file.seekg(0, ios::beg);
        char* buffer = new char[size];
        file.read(buffer, size);

        ofstream out("new_bsp.bsp", ios::binary);
        out.write(buffer, size); // Copy the whole file

        out.seekp(lumpOffset, ios::beg);
        short firstIndex(0); // The index 0 generally corresponds to the nodraw texture
        for (int i = 0; i < lumpLen/BRUSHSIDE_SIZE; i++){
            out.seekp(sizeof(short), ios::cur); // ignore planenum

            out.write(reinterpret_cast<char*>(&firstIndex), sizeof(short)); // texinfo

            out.seekp(sizeof(short), ios::cur); // ignore dispinfo
            out.seekp(sizeof(short), ios::cur); // ignore bevel
        }

        // Free memory
        delete[] buffer;
        out.close();
        file.close();
    }else{
        cout << "Failed to open the file !" << endl;
    }

    return 0;
}