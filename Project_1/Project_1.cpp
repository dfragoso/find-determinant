#include "pch.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
using namespace std;

struct Node
{
	int value;
	int row;
	int colunm;
	Node* next;
};

struct Matrix {
	Node* head_00 = nullptr;
	int side_length_00;

	Matrix(Node* head, int numCol) {
		head_00 = head;
		side_length_00 = numCol;
	}
	int getSize(Node* head) {
		Node* temp = head;
		if (head == nullptr) {
			return 0;
		}
		int nodeCount = 0;
		bool counting = true;
		while (counting) {
			if (temp->next == nullptr) {
				counting = false;
			}
			else {
				temp = temp->next;
			}			
			nodeCount++;			
		}
		return nodeCount;
	}
	int getValue(int row, int colunm) {
		Node* temp = head_00;
		Node* ref = nullptr;
		bool end = false;
		while (!end) {
			if (temp->next == nullptr) {
				end = true;
			}
			if (temp->colunm == colunm && temp->row == row) {
				return temp->value;
			}
			if (temp->next != nullptr) {
				ref = temp->next;
				temp = ref;
			}			
		}		
		return 0;
	}
	
	Node* minor(int row, int colunm) {
		Node* minorHead = nullptr;
		Node* previous = nullptr;
		Node* tempHead = head_00;
		bool notFinish = true;
		while (notFinish) {
			if (tempHead->next == nullptr) {
				notFinish = false;
			}
			if (tempHead->colunm != colunm && tempHead->row != row) {
				Node* newNode = new Node();
				newNode->value = tempHead->value;
				newNode->colunm = tempHead->colunm;
				newNode->row = tempHead->row;

				if (previous != nullptr) {
					previous->next = newNode;
				}
				previous = newNode;

				if (minorHead == nullptr) {
					minorHead = previous;
				}
			}
			tempHead = tempHead->next;
		}
		return minorHead;
	}

	int findDeterminant(Node* matrixHead, int side_length, int ignore_column) {
		int determinant = 0;
		int common_factor = 1;
		int rowCoordinate = 0;
		bool finished = false;
	
		int nodeCount = getSize(matrixHead);

		if (matrixHead == nullptr) { //No matrix at all
			return 0;
		}
		
		if (side_length == 1) { //1by1 matrix
			return matrixHead->value;
		}
		else if (matrixHead->next == nullptr) { //all zeros except one element that is not at (0,0) in the matrix
			return 0;
		}			
		
		Node* temp = nullptr;
		int product1;
		int product2;
		if (nodeCount == 4) {
			product1 = matrixHead->value;
			product2 = matrixHead->next->value;
			temp = matrixHead->next->next;
			product2 *= temp->value;
			product1 *= temp->next->value;

			return product1 - product2;
		}
		if (nodeCount == 2) {
			if (matrixHead->next->row == matrixHead->row + 1 && matrixHead->next->colunm == matrixHead->colunm + 1) {
				return (matrixHead->value * matrixHead->next->value);
			}
			else if (matrixHead->next->row == matrixHead->row + 1 && matrixHead->next->colunm == matrixHead->colunm - 1) {
				return (-(matrixHead->value * matrixHead->next->value));
			}
			else {
				return 0;
			}
		}
		if (nodeCount == 3) {
			if (matrixHead->next->row == matrixHead->row && matrixHead->next->colunm == matrixHead->colunm+1) {
				if (matrixHead->next->next->row == matrixHead->row + 1 && matrixHead->next->next->colunm == matrixHead->colunm) {
					return -(matrixHead->next->value * matrixHead->next->next->value);
				}
				else if (matrixHead->next->next->row == matrixHead->row + 1 && matrixHead->next->next->colunm == matrixHead->colunm + 1) {
					return (matrixHead->value * matrixHead->next->next->value);
				}
			}
			else{
				if (matrixHead->colunm == matrixHead->next->colunm) {
					return (matrixHead->value * matrixHead->next->next->value);
				}
				return -(matrixHead->value * matrixHead->next->value);
			}
		}
		
		for (int colunm = matrixHead->colunm; colunm < side_length_00; colunm++) {
			//Finding the factor that must be multiplied by the minor
			if (colunm != ignore_column) {
				common_factor = pow(-1, matrixHead->row + colunm) * getValue(matrixHead->row, colunm);


				//call findDeterminant recursevly with head of the minor and all the elements at x and y with visited = true;
				determinant += common_factor * findDeterminant(minor(matrixHead->row, colunm), --side_length, colunm);

				/*the determinant formula----->determinant += pow(-1, x+y)getValue(x, y)*findDeterminant(minor(x, y));*/
			}
		}		
		return determinant;
	}
};

int main()
{
	cout << "Matrix: " << endl;
	Node* head = nullptr;
	Node* reference = nullptr;

	////////Reading the matrix in//////////
	int nodeValue = 0;
	int rowLength = 0;
	int nodeCount = 0;
	//coordinates
	int rowNum = 0;
	int colNum = 0;

	string token;
	string matrixRow;

	getline(cin, matrixRow);
	stringstream lineStream(matrixRow);
	while (lineStream >> token)
	{
		colNum++;
	}
	for (int i = 0; i < colNum; i++) {
		stringstream lineStream2(matrixRow);
		rowLength = 0;
		while (lineStream2 >> token)
		{
			nodeValue = stoi(token);
			if (nodeValue != 0) {
				Node* temp = new Node();
				temp->value = nodeValue;
				temp->colunm = rowLength;
				temp->row = rowNum;
				if (reference != nullptr) {
					reference->next = temp;
				}
				reference = temp;

				if (head == nullptr) {
					head = reference;
				}
				nodeCount++;
			}
			rowLength++;
		}
		if (rowLength != colNum) {
			cout << "Error! Non-square matrix!" << endl;
			return 0;
		}
		getline(cin, matrixRow);
		rowNum++;
	}
	if (rowNum != colNum) {
		cout << "Error! Non-square matrix!" << endl;
		return 0;
	}
	/////////////////////////////////////////////////

	Matrix theMatrix = Matrix(head, colNum);
	cout << "Determinant: " << theMatrix.findDeterminant(theMatrix.head_00, colNum, 0);

	return 0;
};
