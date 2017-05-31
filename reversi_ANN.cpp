/*
all map format (Y,X) function format (x,y)
copyright(c) 2017 MAGNET inc.
YOU O:2   COM @:2
   X 0 1 2 3 4 5 6 7
 Y ------------------
 0|                  |
 1|                  |
 2|                  |
 3|        O @       |
 4|        @ O       |
 5|                  |
 6|                  |
 7|                  |
   ------------------
*/
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream> //file
#include <iomanip>//setprecision
#include <math.h>//pow()
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;
char version [128] = "ANN test version.";
int com();
double sigmoid(double x1);
double dsigmoid(double x1);
double logit(double x1);
double sigmoid(double x1) {
  double answer;
  answer = (1.0) / (1 + pow(2.7182818284590452353602874713527, -x1));
  return answer;
}
double dsigmoid(double x1) {
  double answer;
  answer = (pow(2.7182818284590452353602874713527, -x1)) / pow((1 + pow(2.7182818284590452353602874713527, -x1)), 2);
  if(isnan(answer)) {
    answer = 0;
  }
  return answer;
}
double logit(double x1) {
  double answer;
  answer = log(x1 / (1 - (x1)));
  return answer;
}
class matrix {
  public:
    matrix();
    matrix(const matrix& m1);
    matrix& operator =(const matrix& m1);
    ~matrix();
    matrix(int row, int column);
    matrix(int row, int column, double value);
    matrix(int row, int column, double* value);
    int setmatrix(int row, int column, double* a);
    int setmatrix(double* a);
    int get_row();
    int get_column();
    double get_element(int row, int column);
    matrix get_row_as_matrix(int row);
    int print();
    int save_to_file(string filename);
    int load_from_file(string filename);
    double length();
    matrix transfer(double (*function)(double));
    matrix transpose();
    friend matrix multi(const matrix& m1, const matrix& m2);
    friend matrix operator +(const matrix& m1, const matrix& m2);
    friend matrix operator -(const matrix& m1, const matrix& m2);
    friend matrix operator *(const matrix& m1, const matrix& m2);
    friend matrix operator *(const double& x1, const matrix& m1);
    friend matrix operator *(const matrix& m1, const double& x1);
    friend matrix operator /(const double& x1, const matrix& m1);
    friend matrix operator /(const matrix& m1, const double& x1);
    friend matrix operator -(const matrix& m1);
    friend bool operator ==(const matrix& m1, const matrix& m2);
    friend ostream& operator<<(ostream &out, const matrix& m1);
    friend istream& operator>>(istream &in, matrix& m1);
  private:
    int row, column;
    double *a;
};
matrix::matrix() {
  this->row = 1;
  this->column = 1;
  this->a = new double[1];
  a[0] = 0;
}
matrix& matrix::operator =(const matrix& m1) {
  if(&m1 == this)
    return *this;
  int i, j;
  delete [] this->a;
  this->a = new double[m1.row * m1.column];
  this->row = m1.row;
  this->column = m1.column;
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          this->a[i * m1.column + j] = m1.a[i * m1.column + j];
      }
  }
  return *this;
}
matrix::matrix(const matrix& m1) {
  if(&m1 != this) {
    int i, j;
    // delete [] this->a;
    this->a = new double[m1.row * m1.column];
    this->row = m1.row;
    this->column = m1.column;
    for(i = 0; i < m1.row; i++) {
        for(j = 0; j < m1.column; j++) {
            this->a[i * m1.column + j] = m1.a[i * m1.column + j];
        }
    }
  }
}
matrix::~matrix() {
  delete [] a;
}
matrix::matrix(int row, int column) {
  srand (time(NULL));
  int i, j;
  this->a = new double[row * column];
  this->row = row;
  this->column = column;
  for(i = 0; i < this->row; i++) {
      for(j = 0; j < this->column; j++) {
        this->a[i * this->column + j] = rand() % 5 - rand() % 5;
      }
  }
}
matrix::matrix(int row, int column, double value) {
  int i, j;
  this->a = new double[row * column];
  this->row = row;
  this->column = column;
  for(i = 0; i < this->row; i++) {
      for(j = 0; j < this->column; j++) {
        this->a[i * this->column + j] = value;
      }
  }
}
matrix::matrix(int row, int column, double* value) {
  int i, j;
  this->a = new double[row * column];
  this->row = row;
  this->column = column;
  for(i = 0; i < this->row; i++) {
      for(j = 0; j < this->column; j++) {
        this->a[i * this->column + j] = value[i * this->column + j];
      }
  }
}
int matrix::setmatrix(int row, int column, double* a) {
  int i, j;
  delete [] this->a;
  this->a = new double[row * column];
  this->row = row;
  this->column = column;
  for(i = 0; i < row; i++) {
    for(j = 0; j < column; j++) {
      this->a[i * this->column + j] = a[i * this->column + j];
    }
  }
  return 0;
}
int matrix::setmatrix(double* a) {
  int i, j;
  for(i = 0; i < this->row; i++) {
    for(j = 0; j < this->column; j++) {
      this->a[i * this->column + j] = a[i * this->column + j];
    }
  }
  return 0;
}
int matrix::get_row() {
  return this->row;
}
int matrix::get_column() {
  return this->column;
}
double matrix::get_element(int row, int column) {
  if((row - 1) * this->column + (column) > this->column * this->row) {
    cout << "matrix error: get_element" << endl;
  }
  return this->a[(row - 1) * this->column + (column - 1)];
}
matrix matrix::get_row_as_matrix(int row) {
  int i;
  matrix answer(1, this->column);
  double elements[this->column];
  for(i = 0; i < this->column; i++) {
    elements[i] = a[(row - 1) * this->column + i];
  }
  answer.setmatrix(1, this->column, elements);
  return answer;
}
int matrix::print() {
  int i, j;
  cout << ">>>" << this->row << " by " << this->column << " matrix." << endl;
  cout << ">>>    ";
  for(j = 0; j < this->column; j++) {
    cout << "c" << j + 1 <<"  ";
    if (j + 1 < 10) {
      cout << " ";
    }
  }
  cout << endl;
  for(i = 0; i < this->row; i++) {
    cout << ">>>r" << i + 1 << " ";
      for(j = 0; j < this->column; j++) {
        cout << fixed;
        cout << setprecision(2);
        if(this->a[i * this->column + j] >= 0) {
          cout << " ";
        }
        cout << this->a[i * this->column + j];
      }
      cout << endl;
  }
  return 0;
}
int matrix::save_to_file(string filename) {
  ofstream myfile (filename + ".mtrx");
  myfile <<  (*this);
  return 0;
}
int matrix::load_from_file(string filename) {
  ifstream myfile (filename + ".mtrx");
  if(myfile.is_open()) {
      myfile >> (*this);
  }
  else {
    return -1;
  }
  return 0;
}
double matrix::length() {
  double sum = 0;
  int i, j;
  for(i = 0; i < this->row; i++) {
    for(j = 0; j < this->column; j++) {
      sum += pow(this->a[i * this->column + j], 2);
    }
  }
  return sqrt(sum);
}
matrix matrix::transfer(double (*function)(double)) {
  matrix answer(this->row, this->column);
  int i, j;
  for(i = 0; i < this->row; i++) {
      for(j = 0; j < this->column; j++) {
          answer.a[i * this->column + j] = (*function)(this->a[i * this->column + j]);
      }
  }
  return answer;
}
matrix matrix::transpose(){
  matrix answer(this->column, this->row);
  int i, j;
  for(i = 0; i < this->column; i++) {
      for(j = 0; j < this->row; j++) {
          answer.a[i * this->row + j] = this->a[j * this->column + i];
      }
  }
  return answer;
}
matrix multi(const matrix& m1, const matrix& m2) {
  if (m1.row != m2.row || m1.column != m2.column) {
    cout << "matrix error: multi" << endl;
    matrix err(0, 0);
    return err;
  }
  int i, j;
  matrix answer(m1.row, m1.column);
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          answer.a[i * m1.column + j] = m1.a[i * m1.column + j] * m2.a[i * m1.column + j];
      }
  }
  return answer;
}
matrix operator +(const matrix& m1, const matrix& m2) {
  if (m1.row != m2.row || m1.column != m2.column) {
    cout << "matrix error: operator +" << endl;
    matrix err(0, 0);
    return err;
  }
  int i, j;
  matrix answer(m1.row, m1.column);
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          answer.a[i * m1.column + j] = m1.a[i * m1.column + j] + m2.a[i * m1.column + j];
      }
  }
  return answer;
}
matrix operator -(const matrix& m1, const matrix& m2) {
  if (m1.row != m2.row || m1.column != m2.column) {
    cout << "matrix error: operator -" << endl;
    matrix err(0, 0);
    return err;
  }
  int i, j;
  matrix answer(m1.row, m1.column);
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          answer.a[i * m1.column + j] = m1.a[i * m1.column + j] - m2.a[i * m1.column + j];
      }
  }
  return answer;
}
matrix operator *(const matrix& m1, const matrix& m2) {
  if (m1.column != m2.row) {
    cout << "matrix error: operator *" << endl;
    matrix err(0, 0);
    return err;
  }
  matrix answer(m1.row, m2.column);
  int i, j, k;
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m2.column; j++) {
          double temp = 0;
          for(k = 0; k < m1.column; k++) {
              temp += m1.a[i * m1.column + k] * m2.a[k * m2.column + j];
          }
          answer.a[i * m2.column + j] = temp;
      }
  }
  return answer;
}
matrix operator *(const double& x1, const matrix& m1) {
  int i, j;
  matrix answer(m1.row, m1.column);
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          answer.a[i * m1.column + j] = x1 * m1.a[i * m1.column + j];
      }
  }
  return answer;
}
matrix operator *(const matrix& m1, const double& x1) {
  int i, j;
  matrix answer(m1.row, m1.column);
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          answer.a[i * m1.column + j] = x1 * m1.a[i * m1.column + j];
      }
  }
  return answer;
}
matrix operator /(const double& x1, const matrix& m1) {
  int i, j;
  matrix answer(m1.row, m1.column);
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          answer.a[i * m1.column + j] = x1 / m1.a[i * m1.column + j];
      }
  }
  return answer;
}
matrix operator /(const matrix& m1, const double& x1) {
  int i, j;
  matrix answer(m1.row, m1.column);
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          answer.a[i * m1.column + j] = m1.a[i * m1.column + j] / x1;
      }
  }
  return answer;
}
matrix operator -(const matrix& m1) {
  int i, j;
  matrix answer(m1.row, m1.column);
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          answer.a[i * m1.column + j] = -m1.a[i * m1.column + j];
      }
  }
  return answer;
}
bool operator ==(const matrix& m1, const matrix& m2) {
  int i, j;
  bool answer = true;
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          if(m1.a[i * m1.column + j] != m2.a[i * m1.column + j])
            answer = false;
      }
  }
  return answer;
}
ostream& operator<<(ostream &out, const matrix& m1) {
  int i, j;
  out << " " << m1.row << " " << m1.column << endl;;
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
        out << fixed;
        out << setprecision(20);
        out << " " << m1.a[i * m1.column + j];
      }
      out << endl;
  }
  return out;
}
istream& operator>>(istream &in, matrix& m1) {
  int i, j;
  in >> m1.row >> m1.column;
  delete [] m1.a;
  m1.a = new double [m1.row * m1.column];
  for(i = 0; i < m1.row; i++) {
      for(j = 0; j < m1.column; j++) {
          in >> m1.a[i * m1.column + j];
      }
  }
  return in;
}
class ANN {
  public:
    ANN();
    ANN(int layers_size,int *neurons_size);
    ANN(int layers_size,int *neurons_size,matrix *weight);
    ANN(const ANN& ann1);
    ANN& operator =(const ANN& ann1);
    friend ANN operator +(const ANN& ann1, const ANN& ann2);
    friend ANN operator -(const ANN& ann1, const ANN& ann2);
    friend ANN operator /(const ANN& ann1, double x1);
    friend ANN operator *(double x1, const ANN& ann1);
    friend ANN operator *(const ANN& ann1, double x1);
    friend bool operator ==(const ANN& ann1, const ANN& ann2);
    ~ANN();
    int setweight(matrix *weight);
    int randomweight();
    int print();
    int print_detail();
    int save_to_file(string filename);
    int load_from_file(string filename);
    int train(matrix input, matrix output, double speed);
    int train_pro(matrix input, matrix output, double err, int max_times, double speed, int loop, string ann_name);
    matrix feed(matrix input);
  private:
    int layers_size, *neurons_size;
    matrix *weight;
};
ANN::ANN () {
  this->layers_size = 1;
  this->neurons_size = new int[1];
  this->weight = new matrix[1];
}
ANN::ANN(int layers_size,int *neurons_size) {
  int i, j, neurons_size_sum = 0;
  this->layers_size = layers_size;
  this->neurons_size = new int[layers_size];
  this->weight = new matrix[layers_size - 1];
  for(i = 0; i < layers_size; i++) {
    this->neurons_size[i] = neurons_size[i];
    if(i < layers_size -1) {
      matrix new_matrix(neurons_size[i], neurons_size[i + 1]);
      this->weight[i] = new_matrix;
    }
  }
}
ANN::ANN(int layers_size,int *neurons_size,matrix *weight) {
  int i, j, neurons_size_sum = 0;
  this->layers_size = layers_size;
  this->neurons_size = new int[layers_size];
  this->weight = new matrix[layers_size - 1];
  for(i = 0; i < this->layers_size; i++) {
    this->neurons_size[i] = neurons_size[i];
    if(i < this->layers_size -1) {
      if (weight[i].get_row() != neurons_size[i] || weight[i].get_column() != neurons_size[i + 1])
        cout << "ann error: weights not fit neurons size" << endl;
      this->weight[i] = weight[i];
    }
  }
}
ANN::ANN(const ANN& ann1) {
  int i, j, neurons_size_sum = 0;
  this->layers_size = ann1.layers_size;
  this->neurons_size = new int[ann1.layers_size];
  this->weight = new matrix[ann1.layers_size - 1];
  for(i = 0; i < this->layers_size; i++) {
    this->neurons_size[i] = ann1.neurons_size[i];
    if(i < this->layers_size -1) {
      this->weight[i] = ann1.weight[i];
    }
  }
}
ANN& ANN::operator =(const ANN& ann1) {
  if(&ann1 == this)
    return *this;
  int i, j, neurons_size_sum = 0;
  delete [] neurons_size;
  delete [] weight;
  this->layers_size = ann1.layers_size;
  this->neurons_size = new int[ann1.layers_size];
  this->weight = new matrix[ann1.layers_size - 1];
  for(i = 0; i < this->layers_size; i++) {
    this->neurons_size[i] = ann1.neurons_size[i];
    if(i < this->layers_size -1) {
      this->weight[i] = ann1.weight[i];
    }
  }
  return *this;
}
ANN operator +(const ANN& ann1,const ANN& ann2) {
  int i, j;
  ANN answer;
  if(ann1 == ann2) {
    answer = ann1;
    for(i = 0; i < ann1.layers_size - 1; i++) {
      answer.weight[i] = ann1.weight[i] + ann2.weight[i];
    }
  }
  else {
    cout << "ann error: operator -" << endl;
  }
  return answer;
}
ANN operator -(const ANN& ann1,const ANN& ann2) {
  int i, j;
  ANN answer;
  if(ann1 == ann2) {
    answer = ann1;
    for(i = 0; i < ann1.layers_size - 1; i++) {
      answer.weight[i] = ann1.weight[i] - ann2.weight[i];
    }
  }
  else {
    cout << "ann error: operator -" << endl;
  }
  return answer;
}
ANN operator /(const ANN& ann1, double x1) {
  int i, j;
  ANN answer;
  answer = ann1;
  if(x1 == 0) {
    cout << "ann error: operator /" << endl;
  }
  for(i = 0; i < ann1.layers_size - 1; i++) {
    answer.weight[i] = ann1.weight[i] / x1;
  }
  return ann1;
}
ANN operator *(double x1,const ANN& ann1) {
  int i, j;
  ANN answer;
  answer = ann1;
  for(i = 0; i < ann1.layers_size; i++) {
    answer.weight[i] = ann1.weight[i] * x1;
  }
  return ann1;
}
ANN operator *(const ANN& ann1, double x1) {
  int i, j;
  ANN answer;
  answer = ann1;
  for(i = 0; i < ann1.layers_size - 1; i++) {
    answer.weight[i] = ann1.weight[i] * x1;
  }
  return ann1;
}
bool operator ==(const ANN& ann1, const ANN& ann2) {
  int i, j;
  bool answer = true;
  if(ann1.layers_size != ann2.layers_size) {
    answer = false;
  }
  else {
    for(i = 0; i < ann1.layers_size; i++) {
      if(ann1.neurons_size[i] != ann2.neurons_size[i]) {
        answer = false;
      }
    }
  }
  return answer;
}
ANN::~ANN () {
  delete [] weight;
  delete [] neurons_size;
}
int ANN::setweight(matrix *weight) {
  int i, j;
  for(i = 0; i < this->layers_size; i++) {
    if(i < this->layers_size -1) {
      if (weight[i].get_row() != this->neurons_size[i] || weight[i].get_column() != this->neurons_size[i + 1])
        cout << "ann error: weights not fit neurons size" << endl;
      this->weight[i] = weight[i];
    }
  }
  return 0;
}
int ANN::randomweight() {
  int i, j;
  for(i = 0; i < this->layers_size - 1; i++) {
    matrix new_matrix(this->neurons_size[i], this->neurons_size[i + 1]);
    this->weight[i] = new_matrix;
  }
  return 0;
}
int ANN::print_detail() {
  int i, j;
  cout<<">>>-----ANN info detail-----"<<endl;
  for(i = 0; i < this->layers_size; i++) {
    cout<<">>>";
    cout << "N(" <<  i << "): " << this->neurons_size[i]<<endl;
    if(i < this->layers_size -1) {
      this->weight[i].print();
    }
  }
  return 0;
}
int ANN::print() {
  int i, j;
  cout<<">>>-----ANN info-----"<<endl;
  cout<<">>>";
  cout << "layers size: " << this->layers_size << endl;
  for(i = 0; i < this->layers_size; i++) {
    cout<<">>>";
    cout << "N(" <<  i << "): " << this->neurons_size[i]<<endl;
  }
  return 0;
}
int ANN::save_to_file(string filename) {
  int i, j;
  ofstream myfile (filename + ".ann");
  myfile << this->layers_size;
  for(i = 0; i < this->layers_size; i++) {
    myfile << " " << this->neurons_size[i];
  }
  myfile << endl;
  for(i = 0; i < this->layers_size - 1; i++) {
    myfile << this->weight[i];
  }
  myfile.close();
  return 0;
}
int ANN::load_from_file(string filename) {
  int i, j;
  ifstream myfile (filename + ".ann");
  if(myfile.is_open()) {
    delete [] this->neurons_size;
    delete [] this->weight;
    myfile >> this->layers_size;
    this->neurons_size = new int[this->layers_size];
    this->weight = new matrix[this->layers_size - 1];
    for(i = 0; i < this->layers_size; i++) {
      myfile >> this->neurons_size[i];
    }
    for(i = 0; i < this->layers_size - 1; i++) {
      myfile >> this->weight[i];
    }
  }
  else {
    return -1;
  }
  return 0;
}
int ANN::train(matrix input, matrix output,double speed) {
  if (input.get_column() != this->neurons_size[0]) {
    cout << "ann error: neurons size not fit" << endl;
    matrix err(0, 0);
    return -1;
  }
  int i, j;
  matrix delta[this->layers_size], dj_dweight[this->layers_size - 1], a[this->layers_size], z[this->layers_size];
  matrix a1, a2;
  z[0] = input;
  a1 = input.transfer(sigmoid);
  for(i = 0; i < this->layers_size -1; i++) {
    a[i] = a1;
    z[i + 1] = a1 * weight[i];
    a2 = (a1 * weight[i]).transfer(sigmoid);
    a1 = a2;
  }
  delta[this->layers_size - 1] = multi(-(output.transfer(sigmoid) - a1), z[this->layers_size - 1].transfer(dsigmoid));
  for(i = this->layers_size - 1; i > 0 ; i--) { // i start max layers_size
    dj_dweight[i - 1] = a[i - 1].transpose() * delta[i];
    delta[i - 1] = multi(delta[i] * weight[i - 1].transpose(), z[i - 1].transfer(dsigmoid));
  }
  for(i = 0; i < this->layers_size - 1; i++) {
    this->weight[i] = this->weight[i] - speed * dj_dweight[i];
  }
  return 0;
}
int ANN::train_pro(matrix input, matrix output, double err, int max_times, double speed, int loop, string ann_name) {
  int i, j, k;
  ANN good;
  double speed_max = speed;
  // this->train(input, output, speed);
  double good_err = 99999, firsterr = (this->feed(input) - output.transfer(sigmoid)).length();
  int count = 0;
  int use_cache = 0;
  int cache_count = 0;
  matrix weight_cache[this->layers_size];

  // optimize
  for(i = 0; i < 2; i++){
    cout << ">>>optimizing training(" << i + 1 << "/2)..." << endl;
    for(j = 0; j < input.get_row(); j++) {
      for(k = 0; k < 300; k++) {
        this->train(input.get_row_as_matrix(j + 1), output.get_row_as_matrix(j + 1), 0.3);
      }
      cout << "(" << j + 1<< "/" << input.get_row() << ")..." << flush;
    }
    cout << endl;
  }
  // optimize
  cout << "[ " << fixed << setprecision(6) << firsterr << " first ] ";
  cout << "********************************************************************************" << endl;
  good = (*this);
  while((this->feed(input) - output.transfer(sigmoid)).length() > err && (count < max_times || max_times == -1)) {
    if(count % loop == 0) {
      use_cache = 0;
      cout << fixed;
      cout << setprecision(6);
      if ( good_err == (this->feed(input) - output.transfer(sigmoid)).length()) {
        cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " same  ] ";
        speed =  double(rand() % (int(speed_max + 1))  - (rand() % 99999) * 0.00001);
      }
      else if ( good_err > (this->feed(input) - output.transfer(sigmoid)).length()) {
	      // cout << "cache used: " << cache_count << " ";
	      // cache_count = 0;
	      for(i = 0; i < this->layers_size - 1; i++) {
	        weight_cache[i] = this->weight[i] - good.weight[i];
	        use_cache = 1;
	      }
        while(use_cache == 1) {
          int i;
          double err_temp = 0;
          err_temp = ((this->feed(input) - output.transfer(sigmoid)).length());
          for(i = 0; i < layers_size - 1; i++) {
            this->weight[i] = this->weight[i] + weight_cache[i];
          }
          if((this->feed(input) - output.transfer(sigmoid)).length() >= err_temp) {
            for(i = 0; i < layers_size - 1; i++) {
	            this->weight[i] = this->weight[i] - weight_cache[i];
            }
            //cout << "bad_cache result: " << (this->feed(input) - output.transfer(sigmoid)).length() << endl;
	          use_cache = 0;
          }
          else {
            cache_count++;
          }
        };
        if(cache_count > 0) {
          cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " boost ] ";
        }
        else {
          cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " o.k.  ] ";
        }
        good_err = (this->feed(input) - output.transfer(sigmoid)).length();
        good = (*this);
      }
      else if ( good_err < (this->feed(input) - output.transfer(sigmoid)).length()) {
        cout << "[ " << fixed << setprecision(6) << (this->feed(input) - output.transfer(sigmoid)).length() << " bad   ] ";
        (*this) = good;
        speed =  double(rand() % (int(speed_max + 1))  - (rand() % 99999) * 0.00001);
      }
      for (int i = 0; i < ((this->feed(input) - output.transfer(sigmoid)).length() / firsterr) * 80; i++) {
        if(i == 100) {
          break;
        }
        cout << "*";
      }
      cout << endl;
      good.save_to_file(ann_name + "_latest");
    }
    this->train(input, output, speed);
    count ++;
  }
  this->print();
  cout << ">>>-----origin out-----" << endl;
  (output).print();
  cout << ">>>-----feed out-----" << endl;
  ((this->feed(input)).transfer(logit)).print();
  cout << ">>>-----err value-----" << endl;
  cout << fixed;
  cout << setprecision(10);
  cout << ">>>" << (this->feed(input) - output.transfer(sigmoid)).length() << endl;
  cout << ">>>-----try times-----" << endl;
  cout << count << endl;
  if (count < max_times) {
    return count;
  }
  else {
    return -1;
  }
}
matrix ANN::feed(matrix input) {
  if (input.get_column() != this->neurons_size[0]) {
    cout << "ann error: neurons size not fit" << endl;
    matrix err(0, 0);
    return err;
  }
  int i, j;
  matrix a1, a2;
  a1 = input.transfer(sigmoid);
  for(i = 0; i < this->layers_size -1 ; i++) {
    a2 = (a1 * this->weight[i]).transfer(sigmoid);
    a1 = a2;
  }
  return a1;
}
class reversi {
  private:
    int map[8][8];
    int benefit_map[8][8];
    int benefit(int, int, int, int, int);
    bool check(int, int, int, int, int, int); //recursion
  public:
    reversi();
    int clr(); int print();
    int value(int, int);
    int set(int, int, int);
    int mypoint()    ; int compoint();
    int getbenefit(int, int, int);
    int generatekey(char *&key);
    int setbykey(char *key);
    int printkey();
    int can_set(int);
};
reversi::reversi() {}
int reversi::printkey () {
  char *A=new char[65];
  (*this).generatekey(A);
  cout<<"0123456701234567012345670123456701234567012345670123456701234567"<<endl;
  cout<<A<<endl;
  return 0;
}
int reversi::can_set (int sti) {
  int i=0, j=0;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if((*this).getbenefit(i, j, sti) != -1 ) {
        return 1;
      }
    }
  }
  return 0;
}
int reversi::generatekey (char *&key) {
  int i,j;
  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      switch ((*this).map[j][i]) {
        case 1:
          key[i*8+j]='A';
        break;
        case 0:
          key[i*8+j]='N';
        break;
        case -1:
          key[i*8+j]='B';
        break;
      }
    }
  }
  key[64]='\0';
  return 0;
}
int reversi::setbykey (char *key) {
  int i,j;
  for (i=0; i<8; i++) {
    for (j=0; j<8; j++) {
      switch (key[i*8+j]) {
        case 'A':
          (*this).map[j][i]=1;
        break;
        case 'N':
          (*this).map[j][i]=0;
        break;
        case 'B':
          (*this).map[j][i]=-1;
        break;
      }
    }
  }
  return 0;
}
int reversi::mypoint() {
    int temp=0, i, j;
    for (i=0; i<8; i++) {for (j=0; j<8; j++) {if(map[i][j] == 1)temp++; }}
    return temp;
}
int reversi::compoint() {
    int temp=0, i, j;
    for (i=0; i<8; i++) {for (j=0; j<8; j++) {if(map[i][j] == -1)temp++; }}
    return temp;
}
int reversi::clr() {
    int i, j;
    for (i=0; i<8; i++) { for (j=0; j<8; j++)  { map[i][j]=0; } }
    map[3][3]=1 ;  map[3][4]=-1 ;  map[4][3]=-1 ;  map[4][4]=1;//default
    return 0;
}
int reversi::value(int x, int y) {return map[y][x]; }
int reversi::print() {
    int i, j;
    printf("copyright(c) 2017 MAGNET inc.\nHelp: first input X , press \"Enter\",  and then input Y.\nYOU O:%d   COM @:%d \n", mypoint(), compoint());
    printf("\n");
    printf("   X 0 1 2 3 4 5 6 7 \n");
    printf(" Y ------------------ \n");
    for (i=0; i<8; i++) {
        printf("%2d| ", i);
        for (j=0; j<8; j++) {
            switch(map[i][j]) {
                case 1:
                    printf(" O");
                    break;
                case 0:
                    printf("  ");
                    break;
                case -1:
                    printf(" @");
                    break;
            }
        }
        printf(" |\n");
    }
    printf("   ------------------\n");
    return 0;
}
int reversi::set(int X,  int Y,  int sti) {
    bool temp=0;
    if (map[Y][X] == 0) {
        if (map[Y-1][X-1] == sti*-1 && (*this).check(X-1,  Y-1, -1, -1, sti, 1)) {temp=1; }
        if (map[Y-1][X] == sti*-1 && (*this).check(X, Y-1, 0, -1, sti, 1)) {temp=1; }
        if (map[Y-1][X+1] == sti*-1 && (*this).check(X+1, Y-1, 1, -1, sti, 1)) {temp=1; }
        if (map[Y][X-1] == sti*-1 && (*this).check(X-1, Y, -1, 0, sti, 1)) {temp=1; }
        if (map[Y][X+1] == sti*-1 && (*this).check(X+1, Y, 1, 0, sti, 1)) {temp=1; }
        if (map[Y+1][X-1] == sti*-1 && (*this).check(X-1, Y+1, -1, 1, sti, 1)) {temp=1; }
        if (map[Y+1][X] == sti*-1 && (*this).check(X, Y+1, 0, 1, sti, 1)) {temp=1; }
        if (map[Y+1][X+1] == sti*-1 && (*this).check(X+1, Y+1, 1, 1, sti, 1)) {temp=1; }
    }
    if(temp == 1) {map[Y][X]=sti; }else {return -1;}
    if(temp == 1)return 0;
    return 1;
}
bool reversi::check(int x, int y, int nx, int ny, int sti, int set) {//(start X, Y) (Next X, Y) (mychess) (set or not)
    if(x<0||x>7||y<0||y>7) {return 0; }
    if((*this).value(x, y) == 0)  {return 0; }
    if((*this).value(x, y) == sti)  {return 1; }
    if((*this).value(x, y) == -sti) {
        bool a=(*this).check(x+nx, y+ny, nx, ny, sti, set);
        if(set && a) {map[y][x]=sti; }
        return a;
    }
    return 0;
}
int reversi::getbenefit (int x, int y, int sti) {
  int i, j;
  for (i=0; i<8; i++) {for(j=0; j<8; j++) {(*this).benefit_map[i][j]=-1; }}
  for (i=0; i<8; i++) {
      for (j=0; j<8; j++) {
          if((*this).value(j, i) == sti) {
              (*this).benefit(j, i, -1, -1, sti); (*this).benefit(j, i,  0, -1, sti);  (*this).benefit(j, i,  1, -1, sti);
              (*this).benefit(j, i, -1,  0, sti);                              (*this).benefit(j, i,  1,  0, sti);
              (*this).benefit(j, i, -1,  1, sti); (*this).benefit(j, i,  0,  1, sti);  (*this).benefit(j, i,  1,  1, sti);
          }
      }
  }
  return (*this).benefit_map[y][x];
}
int reversi::benefit(int x, int y, int nx, int ny,int sti) {
    int value=0;
    bool temp=1;
    x+=nx; y+=ny;
    while(temp) {
        if(x<8 && x>=0 && y<8 && y>=0) {
            if((*this).value(x, y) == sti) {(*this).benefit_map[y][x]=-1; temp=0; }
            if((*this).value(x, y) == 0) {
              if((*this).benefit_map[y][x]==-1 && value > 0) {
                (*this).benefit_map[y][x]= 0;
              }
              (*this).benefit_map[y][x]+=value; temp=0;
            }
            if((*this).value(x, y) == -sti) {(*this).benefit_map[y][x]=-1; value++; x+=nx; y+=ny; }
        }else {temp=0; }
    }
    return 0;
}
reversi ctrl;
//-------------------------------com----------------------------------
int com() {
  int i=0, j=0, temp[3]= {-1, 0, 0};
  double temp_value=-1;
  for (i=0; i<8; i++) {
    for(j=0; j<8; j++) {
      if(ctrl.getbenefit(i, j, -1) != -1 ) {
        double value=0;
        value=double(ctrl.getbenefit(i, j, -1));
        //corner
        if ((i==0 && j==0)||(i==7 && j==0)||(i==0 && j==7)||(i==7 && j==7)) {
          if (value<20) {
            value=25;
          }
        }
        //subcorder
        if (  (i==1 && j==0)||(i==0 && j==1)||(i==1 && j==1)) {
          if (ctrl.value(0, 0)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==6 && j==0)||(i==6 && j==1)||(i==7 && j==1)) {
          if (ctrl.value(7, 0)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==0 && j==6)||(i==1 && j==6)||(i==1 && j==7)) {
          if (ctrl.value(0, 7)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        if ((i==7 && j==6)||(i==6 && j==6)||(i==6 && j==7)) {
          if (ctrl.value(7, 7)!=-1) {
            value = 0;
          }
          else {
            value*=1.5;
          }
        }
        //inner corner
        if ((i==2 && j==2)||(i==5 && j==2)||(i==2 && j==5)||(i==5 && j==5)) {
          value = value*2.5;
        }
        //inner border
        if (
          (i==2 && j==3)||(i==2 && j==4)||
          (i==3 && j==5)||(i==4 && j==5)||
          (i==5 && j==3)||(i==5 && j==4)||
          (i==3 && j==2)||(i==4 && j==2)
        ) {
          value = value + 2;
        }
        //border
        if (i==0) {
            if (ctrl.value(0, 0) == 1 || ctrl.value(0, 7) == 1) {
              value=0;
            }
        }
        if (i==7) {
            if (ctrl.value(7, 0) == 1 || ctrl.value(7, 7) == 1) {
              value=0;
            }
        }
        if (j==0) {
            if (ctrl.value(0, 0) == 1 || ctrl.value(7, 0) == 1) {
              value=0;
            }
        }
        if (j==7) {
            if (ctrl.value(0, 7) == 1 || ctrl.value(7, 7) == 1) {
              value=0;
            }
        }

        if (value>temp_value) {
          temp_value=value; temp[1]=i; temp[2]=j;
        }
      }
    }
  }
  if(temp_value>=0) {
    ctrl.set(temp[1], temp[2], -1);
  }
  else {
  }
  return 0;
}
//---------------------------reversi_ANN------------------------------
class reversi_ANN {
  public:
    matrix render_outmatrix(double *map_raw, double row, int *column_loc, double *render_value, double distance);
    int train(string filename, double distance);
    int train_ramdom();
};
matrix reversi_ANN::render_outmatrix(double *map_raw, double row, int *column_loc, double *render_value, double distance) {
  int i, j;
  double sum;
  double *map = new double[9999];
  matrix answer;
  sum = 1.0 / (1.0 - distance);
  for(i = 0; i < row * 64; i++) {
    map[i] = map_raw[i];
  }
  for(i = 0; i < row; i++) {
    map[i * 64 + column_loc[i]] = 0;
  }

  for(i = 0; i < row; i++) {
    for(j = i; j >= 0; j--) {
      map[j * 64 + column_loc[j]] += (pow(distance, i - j) / sum) * render_value[i];
    }
  }
  answer.setmatrix(row, 64, map);
  return answer;
}
int reversi_ANN::train(string filename, double distance) {
  ANN myann;
  myann.load_from_file(filename);
  myann.print();
  ctrl.print();
  int count = 0, i, j, x , y, location[65];
  double in[9999], out[9999], render_value[65];
  matrix IN, OUT;
  matrix print;
  while(ctrl.mypoint() + ctrl.compoint() < 64) {
    double in_temp[64], out_temp[64];
    matrix IN_temp;
    matrix OUT_temp;
    for(i = 0; i < 8; i++) {
      for(j = 0; j < 8; j++) {
        in[i * 8 + j + count * 64] = ctrl.value(i, j);
        in_temp[i * 8 + j] = ctrl.value(i, j);
      }
    }
    IN_temp.setmatrix(1, 64, in_temp);
    OUT_temp = (myann.feed(IN_temp)).transfer(logit);
    //clip output
    for(i = 0; i < 8; i++) {
      for(j = 0; j < 8; j++) {
        if(ctrl.getbenefit(j, i, 1) == -1) {
          out[i * 8 + j + count * 64] = -1;
          out_temp[i * 8 + j] = -1;
        }
        else {
          if(OUT_temp.get_element(1, i * 8 + j + 1) <= 1 && OUT_temp.get_element(1, i * 8 + j + 1) >= 0) {
            out_temp[i * 8 + j] = OUT_temp.get_element(1, i * 8 + j + 1);
            out[i * 8 + j + count * 64] = OUT_temp.get_element(1, i * 8 + j + 1);
          }
          else {
            out_temp[i * 8 + j] = 1;
            out[i * 8 + j + count * 64] = 1;
          }
        }
      }
    }
    cout << "clip result: " << endl;
    print.setmatrix(8, 8, out_temp);
    print.print();
    // search small
    double temp_value = -1;
    for (i=0; i<8; i++) {
      for(j=0; j<8; j++) {
        if(out_temp[i * 8 + j] >= 0 ) {
          double value = 0;
          value = out_temp[i * 8 + j];
          if (value > temp_value) {
            temp_value = value;
            x = j;
            y = i;
          }
        }
      }
    }
    if(temp_value >= 0) {
      location[count] = x + y * 8;
      count++;
      cout << "\nstep: " << count << endl;
      ctrl.set(x, y, 1);
      ctrl.print();
      if(ctrl.mypoint() > ctrl.compoint()) {
        render_value[count] = 1;
      }
      else if(ctrl.mypoint() == ctrl.compoint()) {
        render_value[count] = 0.5;
      }
      else {
        render_value[count] = 0;
      }
    }
    com();
    ctrl.print();
  }
  IN.setmatrix(count, 64, in);
  double d;
  // cin >> d;
  d = 0.1;
  OUT = this->render_outmatrix(out, count , location, render_value , distance);
  IN.save_to_file("in");
  OUT.save_to_file("out");
  return 0;
}
//--------------------------------------------------------------------
int main() {
  reversi_ANN myreversi_ANN;
  double distance, error, trytime;
  int count = 0;
  cout << "distance: " << endl;
  cin >> distance;
  cout << "error: " << endl;
  cin >> error;
  cout << "try time: " << endl;
  cin >> trytime;
  while(1) {
    double err;
    char A[65] = "NNNNNNNNNNNNNNNNNNNNNNNNNNNABNNNNNNBANNNNNNNNNNNNNNNNNNNNNNNNNNN";
    ctrl.setbykey(A);
    matrix IN, OUT;
    ANN myann;
    myreversi_ANN.train("reversi" + to_string(count), distance);
    IN.load_from_file("in");
    OUT.load_from_file("out");
    myann.load_from_file("reversi" + to_string(count));
    err = pow(pow(error, 2) * IN.get_row(), 0.5);
    cout << ">>>whole min error: " << fixed << setprecision(6) << err << endl;
    myann.train_pro(IN, OUT, err, trytime, 3, 2500, "reversi" + to_string(count + 1));
    myann.save_to_file("reversi" + to_string(count + 1));
    count++;
  }

    // ctrl.clr();
    // int X, Y, temp=1;
    // char *A=new char[65];
    // string B="NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN";
    // cin>>B;
    // cin>>X>>Y;
    // strcpy(A,B.c_str());
    // ctrl.setbykey(A);
    // if (ctrl.can_set(1) == 0) {
    //   com();
    //   return 0;
    // }
    // if (ctrl.set(X, Y, 1) == -1) {
    //   cout
    //   <<"{\n"
    //   <<"    \"version\": \""<<version<<"\",\n"
    //   <<"    \"message\": \""<<"err"<<"\",\n"
    //   <<"    \"copyright\": \""<<"copyright(c) 2017 NOOXY inc."<<"\"\n"
    //   <<"}";
    //   return 0;
    // }
    // com();
}
