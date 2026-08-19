#include <iostream>
#include <unordered_map>

using namespace std;

int main() {
  unordered_map<string, double> umap;

  // inserting values by using [] operator
  umap["one"] = 1.1;
  umap["two"] = 2.22;
  umap["three"] = 3.333;
  umap["four"] = 4.4444;
  umap["five"] = 5.55555;

  // inserting value by insert function
  umap.insert(make_pair("six", 6.66));

  string key = "four";

  // If key not found in map iterator to end is returned
  if (umap.find(key) == umap.end())
    cout << key << " not found\n\n";

  // If key found then iterator to that key is returned
  else
    cout << "Found " << key << "\n\n";

  key = "one";
  if (umap.find(key) == umap.end())
    cout << key << " not found\n";
  else
    cout << "Found " << key << endl;

  // iterating over all value of umap
  unordered_map<string, double>::iterator itr;
  cout << "\nAll Elements : \n";
  for (itr = umap.begin(); itr != umap.end(); itr++) {
    // itr works as a pointer to pair<string, double>
    // type itr->first stores the key part and
    // itr->second stores the value part
    cout << itr->first << " " << itr->second << endl;
  }

  int n = umap.bucket_count();
  cout << "Total no. of buckets/bunches in which key are organised: " << n
       << endl;
}
