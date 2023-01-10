#include "disjoint_set.hpp"
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <stdexcept>
using namespace std;
using plank::Disjoint_Set;

typedef std::runtime_error SRE;

void usage()
{
  fprintf(stderr, "usage: bin/dj_test [prompt]\n");
  exit(1);
}

void print_commands()
{
  printf("I #      - Initialize a new instance of the disjoint set class with # elements.\n");
  printf("F e      - Call Find(e).\n");
  printf("U s1 s2  - Call Union(s1, s2) -- s1 and s2 must be set id's and not random elements.\n");
  printf("P        - Call Print().\n");
  printf("PE       - Call Print_Equiv().\n");
  printf("L        - Print the links using Get_Links().\n");
  printf("R        - Print the ranks using Get_Ranks() and Get_Set_Ids().\n");
  printf("SZ       - Print the sizes using Get_Sizes() and Get_Set_Ids().\n");
  printf("ID       - Print the set id's using Get_Set_Ids().\n");
  printf("IDI      - Print the set id indices using Get_Set_Id_Indices() and Get_Set_Ids().\n");
  printf("E        - Print the elements of each set using Get_Elements() and Get_Set_Ids().\n");
  printf("?        - Print commands.\n");
  printf("Q        - Quit.\n");
}

int main(int argc, char **argv)
{
  istringstream ss;
  vector <string> sv;
  string line, s;
  Disjoint_Set d;
  const vector <int> *vi;
  const vector <int> *sid;
  const vector < list <int> > *vl;
  list <int>::const_iterator lit;
  size_t i;
  bool first;
  int index;

  if ((argc != 1 && argc != 2) || (argc == 2 && (string) argv[1] == "--help")) usage();  

  while (true) {
    if (argc == 2) {
      printf("%s ", argv[1]);
      fflush(stdout);
    }
    if (!getline(cin, line)) return 0;
    try {
      sv.clear();
      ss.clear();
      ss.str(line);
      while (ss >> s) sv.push_back(s);
      if (sv.size() == 0 || sv[0][0] == '#') {

      } else if (sv[0] == "I") {
        if (sv.size() != 2) throw SRE("I must be called with a number of elements.");
        d.Initialize(atoi(sv[1].c_str()));

      } else if (sv[0] == "F") {
        if (sv.size() != 2) throw SRE("F must be called with a element number.");
        cout << d.Find(atoi(sv[1].c_str())) << endl;

      } else if (sv[0] == "U") {
        if (sv.size() != 3) throw SRE("U must be called with two set ids");
        cout << d.Union(atoi(sv[1].c_str()), atoi(sv[2].c_str())) << endl;

      } else if (sv[0] == "P") {
        d.Print();

      } else if (sv[0] == "PE") {
        d.Print_Equiv();

      } else if (sv[0] == "L") {
        vi = d.Get_Links();
        for (i = 0; i < vi->size(); i++) printf(" %d", vi->at(i));
        printf("\n");

      } else if (sv[0] == "R") {
        vi = d.Get_Ranks();
        sid = d.Get_Set_Ids();
        for (i = 0; i < sid->size(); i++) {
          printf("SID: %2d     Rank: %2d\n", sid->at(i), vi->at(sid->at(i)));
        }

      } else if (sv[0] == "SZ") {
        vi = d.Get_Sizes();
        sid = d.Get_Set_Ids();
        for (i = 0; i < sid->size(); i++) {
          printf("SID: %2d     Size: %2d\n", sid->at(i), vi->at(sid->at(i)));
        }
        printf("\n");

      } else if (sv[0] == "ID") {
        vi = d.Get_Set_Ids();
        for (i = 0; i < vi->size(); i++) printf(" %d", vi->at(i));
        printf("\n");

      } else if (sv[0] == "IDI") {
        vi = d.Get_Set_Id_Indices();
        sid = d.Get_Set_Ids();
        for (i = 0; i < sid->size(); i++) {
          printf("SID: %2d     Index in set_ids(): %2d\n", sid->at(i), vi->at(sid->at(i)));
        }
        printf("\n");

      } else if (sv[0] == "E") {
        vl = d.Get_Elements();
        sid = d.Get_Set_Ids();
        for (i = 0; i < sid->size(); i++) {
          index = sid->at(i);
          printf("SID: %2d: {", index);
          first = true;
          for (lit = vl->at(index).begin(); lit != vl->at(index).end(); lit++) {
            if (!first) printf(",");
            first = false;
            printf("%d", *lit);
          }
          printf("}\n");
        }

      } else if (sv[0] == "?") {
        print_commands();

      } else if (sv[0] == "Q") {
        exit(0);

      } else {
        printf("Unknown command: %s\n", sv[0].c_str());
        printf("Use '?' for a list of commands.\n");
      }
    } catch (const std::runtime_error &e) {
      cout << e.what() << endl;
    }
  }
}
