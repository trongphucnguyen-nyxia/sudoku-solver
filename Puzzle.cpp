#include "Puzzle.h"
#include <iomanip>
int pad = 2;

///////////////////////////////////////////////////////////////////////////
Number::Number() {
  value = -1;
  isPredefined = false;
  FillDomain();
}
Number::Number(int _value) { 
  value = _value;
  isPredefined = false;
  FillDomain();
}

void Number::SetValue(int _value, bool _predefined) { 
  value = _value; 
  isPredefined = _predefined;
}

void Number::ClearDomain() { domain.clear(); }
void Number::FillDomain() {
  domain.clear();
  for(int i=1; i<=9; i++) domain.push_back(i);
}

bool Number::RemoveFromDomain(int i) {
  int domainIndex = -1;
  for(int j=0; j<domain.size(); j++) {
    if(domain[j]==i) {
      domainIndex = j;
      break;
    }
  }//endfor
  if( domainIndex >= 0 ) { //found an index to remove
    domain.erase(domain.begin()+domainIndex);
    return true;
  }
  else
    return false;
}
bool Number::RemoveFromDomain(vector<int> badDomain) {
    for (int i = 0; i < badDomain.size(); i++) {
        int val = badDomain[i];
        bool removed = RemoveFromDomain(val);
        if (removed) return true;
    }
    return false;
}
bool Number::DomainHasValue(int val) {
  for(int i=0; i<domain.size(); i++) {
    if( val == domain[i] ) return true;
  }
  return false;
}  
void Number::PrintDomain() {
  cout << "[ ";
  for(int i=0; i<domain.size(); i++) {
    cout << domain[i] << " ";
  }
  cout << "]" << endl;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
Puzzle::Puzzle(string _puzzleFile, double _puzzleW, double _puzzleH) {
    cout << "constructor" << endl;
    puzzleFile = _puzzleFile;
    puzzleW = _puzzleW;
    puzzleH = _puzzleH;
    numConflicts = 0;

    // initialize everything first
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            data[r][c] = -1;
            dataNumber[r][c].SetValue(-1, false);
            dataNumber[r][c].FillDomain();
        }
    }

    ifstream infile(puzzleFile.c_str());
    if (!infile.is_open()) {
        cout << "ERROR: Could not open puzzle file: " << puzzleFile << endl;
        return;
    }

    string lineStr;
    int i = 0;
    while (getline(infile, lineStr) && i < 9) {
        if ((int)lineStr.length() < 9) continue;

        for (int j = 0; j < 9; j++) {
            char ch = lineStr[j];

            if (ch == '-') {
                data[i][j] = -1;
                dataNumber[i][j].SetValue(-1, false);
                dataNumber[i][j].FillDomain();
            }
            else if (ch >= '1' && ch <= '9') {
                int val = ch - '0';
                data[i][j] = val;
                dataNumber[i][j].SetValue(val, true);
                dataNumber[i][j].ClearDomain();
            }
            else {
                data[i][j] = -1;
                dataNumber[i][j].SetValue(-1, false);
                dataNumber[i][j].FillDomain();
            }
        }
        i++;
    }
    infile.close();

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            cout << setw(3) << data[r][c] << " ";
        }
        cout << endl;
    }

    dx = puzzleW / 9;
    dy = puzzleH / 9;

    UpdateDomains();
}

void Puzzle::SetCellToValue(pair<int, int> cell, int val) {
    if (cell.first < 0 || cell.first >= 9 || cell.second < 0 || cell.second >= 9) return;

    Number& n = dataNumber[cell.first][cell.second];
    if (!n.ValidValue()) {
        n.SetValue(val, false);
        n.ClearDomain();
        moves.push_back(Move("User", cell.first, cell.second, val));
        UpdateDomains();
    }
}

void Puzzle::UndoMove() {
  if( moves.size() > 0 ) {
    Move m = moves.back();
    moves.pop_back();
    Number& n = dataNumber[m.i][m.j];
    n.SetValue(-1, false);
    n.FillDomain();
    UpdateDomains();
  }
}

pair<int,int> Puzzle::CoordsToCell(double tx, double ty) {
  int i=-1;
  int j=-1;
  bool valid_i = false;
  bool valid_j = false;
  if( tx >= 0 && tx<=puzzleW ) {
    j = tx / dx;
    valid_j = true;
  }
  if( ty >= 0 && ty<=puzzleH ) {
    i = ty / dy;
    valid_i = true;
  }
  cout << "CoordsToCell x = " << tx << " y = " << ty << " i=" << i << " j=" << j << endl;
  if( valid_i && valid_j ) {
    return make_pair(i,j);
  }
  else return make_pair(-1,-1);
}

void Puzzle::Draw() {
  //cout << "Puzzle draw. puzzleW=" << puzzleW << " puzzleH=" << puzzleH << endl;
  glColor3f(0, 0, 0);
  //glVertex2d(0,0);
  //glVertex2d(puzzleW,puzzleH);
  /*
  //bottom-line
  glVertex2d(0+pad,0+pad);
  glVertex2d(puzzleW-pad,0+pad);
  //top-line
  glVertex2d(0+pad,puzzleH-pad);
  glVertex2d(puzzleW-pad,puzzleH-pad);
  //left-line
  glVertex2d(0+pad,0+pad);
  glVertex2d(0+pad,puzzleH-pad);
  //right-line
  glVertex2d(puzzleW-pad,0+pad);
  glVertex2d(puzzleW-pad,puzzleH-pad);
  */

  //draw vertical lines
  for(int i=0; i<=9; i++) {
    if( i%3 == 0 ) glLineWidth(4);
    else glLineWidth(1);
    glBegin(GL_LINES);
      glVertex2d(i*dx,0);
      glVertex2d(i*dx,puzzleH);
    glEnd();
  }
  //draw horizontal lines
  for(int i=0; i<=9; i++) {
    if( i%3 == 0 ) glLineWidth(4);
    else glLineWidth(1);
    glBegin(GL_LINES);
      glVertex2d(0,i*dy);
      glVertex2d(puzzleW,i*dy);
    glEnd();
  }

  //DrawCell(0,7);
  //renderBitmapString(200,200,"Font Rendering - Programming Techniques");
  for(int i=0; i<9; i++) 
    for(int j=0; j<9; j++)
      DrawCell(i,j);

  DrawMoves();    
}

void renderBitmapString(int type, float x, float y, const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        //glutBitmapCharacter(font, *c);
	if( type == 1 )
	  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	else
	  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	  //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *c);
	  
    }
}
void Puzzle::DrawCell(int i, int j) {
  //cout << "drawcell i=" << i << " j=" << j << " value= " << dataNumber[i][j].value << endl;
  int number = dataNumber[i][j].value;
  if( number > 0 ) { //draw valid values at center of cell
    double x = dx*j + dx/2;
    double y = dy*i + dy/2;
    std::string s = std::to_string(number);
    char const *pchar = s.c_str();
    glColor3f(0, 0, 0);
    renderBitmapString(1, x, y, pchar); 
  }
  else {//draw domain
    string ds;
    vector<int>& domain = dataNumber[i][j].GetDomain();
    for(int b=0; b<domain.size(); b++) {
      ds += to_string(domain[b]);
      if( b!=domain.size()-1 ) ds+= ",";
    }//endfor b
    double x = dx*j + 0.1*dx;
    double y = dy*i + 0.85*dy;
    char const *pchar = ds.c_str();
    glColor3f(0, 0, 1);
    renderBitmapString(2, x, y, pchar); 
  }
}

void Puzzle::DrawMoves() {
    glColor3f(0, 0, 0);
    double moveX = puzzleW + 5;
    double h = 25;
    double moveY = puzzleH - h;
    int counter = 0;

    if (moves.empty()) return;

    for (int i = (int)moves.size() - 1; i >= 0; i--, counter++) {
        double ty = moveY - counter * h;
        string output = moves[i].type;
        output += "<" + to_string(moves[i].i) + "," + to_string(moves[i].j) + ">" + to_string(moves[i].val);
        renderBitmapString(2, moveX, ty, output.c_str());
    }
}

void Puzzle::UpdateDomains() {
  cout << "UpdateDomains" << endl;
  numConflicts = 0;
  for(int i=0; i<9; i++) {
    for(int j=0; j<9; j++) {
      UpdateDomain(i,j);
      CountConflicts(i, j);
    }//endfor j
  }//endfor i
}

void Puzzle::UpdateDomain(int x, int y) {

    Number& num_xy = dataNumber[x][y];
    cout << "Value=" << num_xy.Value() << " UpdateDomain i=" << x << " j=" << y << endl;
    cout << "Start domain = ";
    num_xy.PrintDomain();

    if (!num_xy.ValidValue()) {

        num_xy.FillDomain();

        //remove the column values from domain
        for (int i = 0; i < 9; i++) {
            if (dataNumber[x][i].ValidValue()) {
                bool updated = num_xy.RemoveFromDomain(dataNumber[x][i].Value());
                if (updated) cout << "[1]removed " << dataNumber[x][i].Value() << endl;
            }
        }

        //remove the row values from domain
        for (int i = 0; i < 9; i++) {
            if (dataNumber[i][y].ValidValue()) {
                bool updated = num_xy.RemoveFromDomain(dataNumber[i][y].Value());
                if (updated) cout << "[2]removed " << dataNumber[i][y].Value() << endl;
            }
        }

        //remove from unit 3x3 part
        int unit_i = x / 3;
        int unit_j = y / 3;
        cout << "unit_i=" << unit_i << " unit_j=" << unit_j << endl;
        for (int i = 3 * unit_i; i < 3 * unit_i + 3; i++) {
            for (int j = 3 * unit_j; j < 3 * unit_j + 3; j++) {
                cout << "i=" << i << " j=" << j << " value = " << dataNumber[i][j].Value() << endl;
                if (dataNumber[i][j].ValidValue()) {
                    bool updated = num_xy.RemoveFromDomain(dataNumber[i][j].Value());
                    if (updated) cout << "[3]removed " << dataNumber[i][j].Value() << endl;
                }
            }//endfor j
        }//endfor i

        cout << "End domain = ";
        num_xy.PrintDomain();
    }
}

void Puzzle::CountConflicts(int x, int y) {
 
  Number& num_xy = dataNumber[x][y];
  cout << "Value=" << num_xy.Value() << " CountConflicts i=" << x << " j=" << y << endl;
  if( num_xy.ValidValue() ) {

    //remove the column values from domain
    for(int i=0; i<9; i++) {
      if( dataNumber[x][i].ValidValue() && i!=y ) {
	if(dataNumber[x][i].Value() == dataNumber[x][y].Value()) numConflicts++;
      }
    }

    //remove the row values from domain
    for(int i=0; i<9; i++) {
      if( dataNumber[i][y].ValidValue() && i!=x ) {
	if(dataNumber[i][y].Value() == dataNumber[x][y].Value()) numConflicts++;
      }
    }

    //remove from unit 3x3 part
    int unit_i = x / 3;
    int unit_j = y / 3;
    //cout << "unit_i=" << unit_i << " unit_j=" << unit_j << endl;
    for(int i=3*unit_i; i<3*unit_i+3; i++) {
      for(int j=3*unit_j; j<3*unit_j+3; j++) {
	//cout << "i=" << i << " j=" << j << " value = " << dataNumber[i][j].Value() << endl;
	if( dataNumber[i][j].ValidValue() && i!=x && j!=y ) {
	  if(dataNumber[i][j].Value() == dataNumber[x][y].Value()) numConflicts++;
	}
      }//endfor j
    }//endfor i
  }

}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
// TODO: Fill in the Search, Inference 1-3 functions.
void Puzzle::Search() {
    cout << "Puzzle::Search()" << endl;

    bool changed = true;
    int safety = 100;

    // Repeatedly apply inferences first
    while (changed && safety-- > 0) {
        int beforeMoves = moves.size();
        int beforeConflicts = numConflicts;

        Inf1();
        Inf3();
        Inf2();
        UpdateDomains();

        changed = (moves.size() != beforeMoves) || (numConflicts != beforeConflicts);
    }

    // If puzzle still not solved, do a simple local/greedy fill:
    // repeatedly place the cell with the smallest domain.
    bool progress = true;
    while (progress) {
        progress = false;
        UpdateDomains();

        if (numConflicts == 0) {
            bool complete = true;
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (!dataNumber[i][j].ValidValue()) {
                        complete = false;
                        break;
                    }
                }
            }
            if (complete) return;
        }

        int bestI = -1, bestJ = -1;
        int bestSize = 100;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (!dataNumber[i][j].ValidValue()) {
                    int ds = dataNumber[i][j].GetDomain().size();
                    if (ds > 0 && ds < bestSize) {
                        bestSize = ds;
                        bestI = i;
                        bestJ = j;
                    }
                }
            }
        }

        if (bestI != -1) {
            vector<int>& d = dataNumber[bestI][bestJ].GetDomain();
            if (!d.empty()) {
                dataNumber[bestI][bestJ].SetValue(d[0], false);
                dataNumber[bestI][bestJ].ClearDomain();
                moves.push_back(Move("Search", bestI, bestJ, d[0]));
                UpdateDomains();
                progress = true;
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////
// TODO: Inf1 will set single valued domains to the value in that cell
void Puzzle::Inf1() {
    cout << "Puzzle::Inf1()" << endl;

    bool changed = false;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Number& n = dataNumber[i][j];

            if (!n.ValidValue()) {
                vector<int>& d = n.GetDomain();
                if (d.size() == 1) {
                    int val = d[0];
                    n.SetValue(val, false);
                    n.ClearDomain();
                    moves.push_back(Move("Inf1", i, j, val));
                    changed = true;
                }
            }
        }
    }

    if (changed) UpdateDomains();
}

/////////////////////////////////////////////////////////////////////////
// TODO: Inf2 will try to find like domains and remove those values
//from other's domains


vector< vector< pair<int,int> > > groupIndices(int x, int y) {
  //cout << "groupIndices x=" << x << " y=" << y << endl;
  vector< vector< pair<int,int> > > setIndices;
  //the column indicies
  vector< pair<int,int> > columnIndices;
  for(int i=0; i<9; i++) {
    if( i!=y ) columnIndices.push_back( make_pair(x,i) );
  }

  //the row indices
  vector< pair<int,int> > rowIndices;
  for(int i=0; i<9; i++) {
    if( i!=x ) rowIndices.push_back( make_pair(i,y) );
  }

  //unit (3x3 part) indices
  int unit_i = x / 3;
  int unit_j = y / 3;
  vector< pair<int,int> > unitIndices;
  //cout << "unit_i=" << unit_i << " unit_j=" << unit_j << endl;
  for(int i=3*unit_i; i<3*unit_i+3; i++) {
    for(int j=3*unit_j; j<3*unit_j+3; j++) {
      if( x==i && y==j ) { }
      else unitIndices.push_back( make_pair(i,j) );
    }//endfor j
  }//endfor i
  setIndices.push_back(columnIndices);
  setIndices.push_back(rowIndices);
  setIndices.push_back(unitIndices);
  return setIndices;
}

bool InSet(vector<int> set1, int val) {
  bool found = false;
  for(int i=0; i<set1.size(); i++) {
    if( set1[i] == val ) found = true;
  }
  return found;
}
//determine if set2 is a subset of set1
bool IsSubset(vector<int> set1, vector<int> set2) {
  if( set2.size() == 0 ) return false;
  for(int i=0; i<set2.size(); i++) {
    int val = set2[i];
    if( !InSet(set1,val) ) return false;
  }
  return true;
}

int SetIndex(vector< pair<int,int> >& set1, pair<int,int> indices) {
  int index = -1;
  for(int i=0; i<set1.size(); i++) {
    pair<int,int>& s_ij = set1[i];
    if( s_ij.first == indices.first && s_ij.second == indices.second ) return i;
  }
  return index;
}

void Puzzle::Inf2() {
    cout << "Puzzle::Inf2()" << endl;

    // make sure domains are current
    UpdateDomains();

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            Number& cur = dataNumber[x][y];
            if (cur.ValidValue()) continue;

            vector<int> curDomain = cur.GetDomain();
            int domainSize = (int)curDomain.size();

            // Inf2 only makes sense for size 2 or more
            if (domainSize < 2) continue;

            vector< vector< pair<int, int> > > groups = groupIndices(x, y);

            for (int g = 0; g < (int)groups.size(); g++) {
                vector< pair<int, int> > group = groups[g];

                vector< pair<int, int> > matchingCells;
                matchingCells.push_back(make_pair(x, y));

                // find other open cells with the exact same domain
                for (int k = 0; k < (int)group.size(); k++) {
                    int i = group[k].first;
                    int j = group[k].second;

                    if (dataNumber[i][j].ValidValue()) continue;

                    vector<int> otherDomain = dataNumber[i][j].GetDomain();

                    bool sameDomain =
                        ((int)otherDomain.size() == domainSize) &&
                        IsSubset(otherDomain, curDomain) &&
                        IsSubset(curDomain, otherDomain);

                    if (sameDomain) {
                        matchingCells.push_back(make_pair(i, j));
                    }
                }

                // naked subset rule:
                // if N cells share the same domain of size N, remove those values from others
                if ((int)matchingCells.size() == domainSize) {
                    for (int k = 0; k < (int)group.size(); k++) {
                        int i = group[k].first;
                        int j = group[k].second;

                        if (dataNumber[i][j].ValidValue()) continue;

                        bool skip = false;
                        for (int t = 0; t < (int)matchingCells.size(); t++) {
                            if (matchingCells[t].first == i && matchingCells[t].second == j) {
                                skip = true;
                                break;
                            }
                        }
                        if (skip) continue;

                        bool removedAny = false;
                        for (int d = 0; d < domainSize; d++) {
                            if (dataNumber[i][j].RemoveFromDomain(curDomain[d])) {
                                removedAny = true;
                            }
                        }

                        if (removedAny) {
                            moves.push_back(Move("Inf2", i, j, -1));
                            UpdateDomains();
                            return;   // important: stop after one successful reduction
                        }
                    }
                }
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////
// TODO: Inf3 Unique value in domain
//
void Puzzle::Inf3() {
    cout << "Puzzle::Inf3()" << endl;

    // make sure domains are current
    UpdateDomains();

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            Number& cur = dataNumber[x][y];
            if (cur.ValidValue()) continue;

            vector<int> curDomain = cur.GetDomain();
            if (curDomain.empty()) continue;

            for (int d = 0; d < (int)curDomain.size(); d++) {
                int val = curDomain[d];

                // check uniqueness in row
                bool uniqueInRow = true;
                for (int j = 0; j < 9; j++) {
                    if (j == y) continue;
                    if (!dataNumber[x][j].ValidValue() && dataNumber[x][j].DomainHasValue(val)) {
                        uniqueInRow = false;
                        break;
                    }
                }

                // check uniqueness in column
                bool uniqueInCol = true;
                for (int i = 0; i < 9; i++) {
                    if (i == x) continue;
                    if (!dataNumber[i][y].ValidValue() && dataNumber[i][y].DomainHasValue(val)) {
                        uniqueInCol = false;
                        break;
                    }
                }

                // check uniqueness in 3x3 unit
                bool uniqueInUnit = true;
                int unit_i = x / 3;
                int unit_j = y / 3;
                for (int i = 3 * unit_i; i < 3 * unit_i + 3; i++) {
                    for (int j = 3 * unit_j; j < 3 * unit_j + 3; j++) {
                        if (i == x && j == y) continue;
                        if (!dataNumber[i][j].ValidValue() && dataNumber[i][j].DomainHasValue(val)) {
                            uniqueInUnit = false;
                            break;
                        }
                    }
                    if (!uniqueInUnit) break;
                }

                // if unique anywhere, place it
                if (uniqueInRow || uniqueInCol || uniqueInUnit) {
                    cur.SetValue(val, false);
                    cur.ClearDomain();
                    moves.push_back(Move("Inf3", x, y, val));
                    UpdateDomains();
                    return;   // important: stop after one move
                }
            }
        }
    }
}