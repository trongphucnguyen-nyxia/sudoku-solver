#include <GL/glut.h>  // (or others, depending on the system in use)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

////////////////////////////////////////////////////////////////////
class Number {
public:
  Number();
  Number(int _value);
  void SetValue(int _value, bool _predefined);
  void ClearDomain();
  void FillDomain();
  int Value() { return value; }
  bool ValidValue() { return value>0; }
  bool RemoveFromDomain(int i);
  bool RemoveFromDomain(vector<int> badDomain);
  vector<int>& GetDomain() { return domain; }
  bool DomainHasValue(int val);
  void PrintDomain();


  int value;
  bool isPredefined;
  vector<int> domain;
};

struct Move {
  string type;
  int i;
  int j;
  int val;
  Move(string _t, int _i, int _j, int _val) : type(_t), i(_i), j(_j), val(_val) { }
};

////////////////////////////////////////////////////////////////////
void renderBitmapString(int type, float x, float y, const char *string);


////////////////////////////////////////////////////////////////////
class Puzzle {
public:
  Puzzle(string _puzzleFile, double _puzzleW, double _puzzleH);

  void UpdateDomains();
  void UpdateDomain(int x, int y); 
  void CountConflicts(int x, int y); 
  int NumConflicts() { return numConflicts; }

  pair<int,int> CoordsToCell(double tx, double ty);
  void SetCellToValue(pair<int,int> cell, int val);
  void UndoMove();

  void Search();
  void Inf1();
  void Inf2();
  void Inf3();


  void Draw();
  void DrawCell(int i, int j);
  void DrawMoves();
private:
  double puzzleW, puzzleH;
  string puzzleFile;
  int data[9][9];
  Number dataNumber[9][9];

  vector<Move> moves;

  double dx, dy;

  int numConflicts;
};
