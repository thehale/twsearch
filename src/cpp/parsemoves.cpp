#include "parsemoves.h"
#include "solve.h"
#include <iostream>
setval findmove_generously(const puzdef &pd, const char *mvstring) {
   for (int i=0; i<(int)pd.moves.size(); i++)
      if (strcmp(mvstring, pd.moves[i].name) == 0)
         return pd.moves[i].pos ;
   for (int i=0; i<(int)pd.parsemoves.size(); i++)
      if (strcmp(mvstring, pd.parsemoves[i].name) == 0)
         return pd.parsemoves[i].pos ;
   for (int i=0; i<(int)pd.expandedrotations.size(); i++)
      if (strcmp(mvstring, pd.expandedrotations[i].name) == 0)
         return pd.expandedrotations[i].pos ;
   error("! bad move name ", mvstring) ;
   return setval(0) ;
}
setval findmove_generously(const puzdef &pd, string s) {
   return findmove_generously(pd, s.c_str()) ;
}
int findmove(const puzdef &pd, const char *mvstring) {
   for (int i=0; i<(int)pd.moves.size(); i++)
      if (strcmp(mvstring, pd.moves[i].name) == 0)
         return i ;
   error("! bad move name ", mvstring) ;
   return -1 ;
}
int findmove(const puzdef &pd, string mvstring) {
   return findmove(pd, mvstring.c_str()) ;
}
int findmoveorrotation(const puzdef &pd, const char *mvstring) {
   for (int i=0; i<(int)pd.moves.size(); i++)
      if (strcmp(mvstring, pd.moves[i].name) == 0)
         return i ;
   for (int i=0; i<(int)pd.expandedrotations.size(); i++)
      if (strcmp(mvstring, pd.expandedrotations[i].name) == 0)
         return i+pd.moves.size() ;
   error("! bad move or rotation name ", mvstring) ;
   return -1 ;
}
int findmoveorrotation(const puzdef &pd, string mvstring) {
   return findmoveorrotation(pd, mvstring.c_str()) ;
}
void domove(puzdef &pd, setval p, string mvstring) {
   domove(pd, p, findmove(pd, mvstring)) ;
}
void solveit(const puzdef &pd, prunetable &pt, string scramblename, setval &p,
             generatingset *gs) {
   if (quiet == 0) {
      if (scramblename.size())
         cout << "Solving " << scramblename << endl << flush ;
      else
         cout << "Solving" << endl << flush ;
   }
   solve(pd, pt, p, gs) ;
}
vector<int> parsemovelist(const puzdef &pd, const char *scr) {
   vector<int> movelist ;
   string move ;
   for (const char *p=scr; *p; p++) {
      if (*p <= ' ' || *p == ',') {
         if (move.size()) {
            movelist.push_back(findmove(pd, move)) ;
            move.clear() ;
         }
      } else
         move.push_back(*p) ;
   }
   if (move.size())
      movelist.push_back(findmove(pd, move)) ;
   return movelist ;
}
vector<int> parsemoveorrotationlist(const puzdef &pd, const char *scr) {
   vector<int> movelist ;
   string move ;
   for (const char *p=scr; *p; p++) {
      if (*p <= ' ' || *p == ',') {
         if (move.size()) {
            movelist.push_back(findmoveorrotation(pd, move)) ;
            move.clear() ;
         }
      } else
         move.push_back(*p) ;
   }
   if (move.size())
      movelist.push_back(findmoveorrotation(pd, move)) ;
   return movelist ;
}
vector<setval> parsemovelist_generously(const puzdef &pd, const char *scr) {
   vector<setval> movelist ;
   string move ;
   for (const char *p=scr; *p; p++) {
      if (*p <= ' ' || *p == ',') {
         if (move.size()) {
            movelist.push_back(findmove_generously(pd, move)) ;
            move.clear() ;
         }
      } else
         move.push_back(*p) ;
   }
   if (move.size())
      movelist.push_back(findmove_generously(pd, move)) ;
   return movelist ;
}
/*
 *   A rotation is always a grip (uppercase) followed only by 'p'.  There
 *   must not be a prefix or additional suffix.
 */
int isrotation(const char *mv) {
   const char *q = mv ;
   if ((*q == 'x' || *q == 'y' || *q == 'z') &&
       (q[1] == 0 || q[1] == '\'' || q[1] == '2'))
      return true;
   while (*q == '_' || ('A' <= *q && *q <= 'Z'))
      q++ ;
   if (*q != 'v' || q[1] != 0 || mv == q)
      return 0 ;
   else
      return 1 ;
}
