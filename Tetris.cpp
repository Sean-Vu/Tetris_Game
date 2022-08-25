#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>
using namespace std;

wstring tetromino[7];
int nFieldWidth = 12;				// board width
int nFieldHeight = 18;				// board height
unsigned char* pField = nullptr;	// 

int nScreenWidth = 80;
int nScreenHeight = 30;

int Rotate(int px, int py, int r)
{

	// where r is an integer denoting 90, 180, 270 degree rotation
	// rotation
	// index is equal row*width + x
	// 90 degree rot index = 12 + y - (4x)
	// 180 degree rot index = 15 - (y*4) - x
	// 270 = 3 + y(4x)
	switch (r % 4)
	{
	case 0:	return py * 4 + px;		// no rotation 0 degrees
	case 1:	return 12 + py - (px * 4); // 90 degree rot
	case 2: return 15 - (py * 4) - px; // 180 degree rot
	case 3: return 3 - py + (px * 4);
	}
	return 0;
}
bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			// Index of piece if rot
			int pi = Rotate(px, py, nRotation);

			// get index in field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
					{
						return false; // pieces overlap
					}
				}
			}
		}
	}
	return true;
}

int main()
{
	//creating assets
		// first shape 
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	// second shape
	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");
	// third shape
	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");
	// fourth shape
	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");
	// fifth shape
	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");
	//sixth shape
	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");
	// seventh shape
	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");

	pField = new unsigned char[nFieldWidth * nFieldHeight]; // create board
	for (int x = 0; x < nFieldWidth; x++)	// boundary of board
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0; // if boundary or height set to 9 to denote border otherwise set it equal to 0 to denot empty space
		}
	}

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWRitten = 0;

	bool bGameOver = false;

	int nCurrentPiece = 1;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;
	
	bool bKey[4];
	//bool bRotateHold = false; // if user presses down hold key 

	int nSpeed = 20;
	int nSpeedCounter = 0;	// counts game tics ... if counter equals nSpeed for the piece down
	bool bForceDown = false;
	int nPieceCount = 0;	// keeps track of the amount of pieces given ... every ten pieces we increase the difficulty
	int nScore = 0;

	vector<int> vLines;

	// Resize window
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL)
	{
		MoveWindow(hwnd, 200, 200, 680, 400, TRUE);
	}

	while (!bGameOver) // loop while game is not over
	{
		// GAME TIMING 
		
		this_thread::sleep_for(50ms); // one game tic
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == nSpeed);


		// INPUT
		for (int k = 0; k < 4; k++)
		{
																// R   L   D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0; // GetAsyncKeyState checks if key is pressed and if so return true (virtual key codes in hexadecimal)
			// at the end bKey is an array of true or false values which we can check to correspond to left, down, up , or rotate
		}
		
		// GAME LOGIC ( shapes falling, collision, score)
		if (bKey[1])	// user has pressed left key
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY))	// if piece fits update current x variable
			{
				nCurrentX = nCurrentX - 1;	// updates if the piece can fit and shifts left one unit
			}
		}
		if (bKey[0])	// user has pressed left key
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY))	// if piece fits update current x variable
			{
				nCurrentX = nCurrentX + 1;	// updates if the piece can fit and shifts right one unit
			}
		}
		if (bKey[2])	// user has pressed left key
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))	// if piece fits update current y variable
			{
				nCurrentY = nCurrentY + 1;	// updates if the piece can fit and shifts down one unit
			}
		}
		if (bKey[3])
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) // if z key which is rotation is pressed
			{
				nCurrentRotation = nCurrentRotation + 1;	// rotate piece
			}
		}
		
		if (bForceDown)
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			{
				nCurrentY++; // if piece can fit one more down move it down;
			}
			else
			{
				// if piece cant fit down we need to lock piece into the field
				for (int px = 0; px < 4; px++)
				{
					for (int py = 0; py < 4; py++)
					{
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
						{
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
						}
					}
				}
				nPieceCount++; // piece is permanent at this point and we can increment piece count
				if ((nPieceCount % 10) == 0)
				{
					if (nSpeed >= 10)
					{
						nSpeed--; // speed will increase
					}
				}
				
				// check if we have a tetris
				for (int py = 0; py < 4; py++)
				{
					if (nCurrentY + py < nFieldHeight - 1)
					{
						bool bLine = true;	// assume we have a line but if there are any empty spaces set bLine to false
						for (int px = 1; px < nFieldWidth - 1; px++)
						{
							bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;
						}
						if (bLine) // by this point we have a line so increment score! and remove line
						{
							for (int px = 1; px < nFieldWidth - 1; px++)
							{
								pField[(nCurrentY + py) * nFieldWidth + px] = 8; // 8 is the equals symbol

							}
							vLines.push_back(nCurrentY + py);
						}
					}
				}
				nScore += 25;
				if (!vLines.empty())
				{
					nScore += (1 << vLines.size()) * 100; // multiple removed lines gives bigger score
				}
				// choose next piece
				nCurrentX = nFieldWidth / 2;	// take in next piece and randomize which one is picked
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7; 

				// if piece does not fit
				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}
			nSpeedCounter = 0;
		}
		// RENDER OUTPUT


		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
		{
			for (int y = 0; y < nFieldHeight; ++y)
			{
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
				// if index is the 0 pos then empty space
				// if index is the 1 pos then tetris pieces will be "A"
				// if index is the 2 pos then tetris pieces will be "B" ...
			}
		}

		// Draw current Piece 
		for (int px = 0; px < 4; px++)
		{
			for (int py = 0; py < 4; py++)
			{
				if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
				{
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65; // 65 is ascii for A
				}
			}
		}

		// Draw Score
		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);
		if (!vLines.empty())
		{
			// draw lines
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth* nScreenHeight, { 0,0 }, & dwBytesWRitten);
			this_thread::sleep_for(400ms); // delay if no lines 

			// move down all pieces above the line
			for (auto& v : vLines)	// if a line exists we have the line number(row)
			{
				for (int px = 1; px < nFieldWidth - 1; px++)
				{
					for (int py = v; py > 0; py--)
					{
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
					}
					pField[px] = 0;
				}
			}
			vLines.clear(); // clear lines
		}
		// Display border
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWRitten);

		
	}

	// loop will exit if game over

	CloseHandle(hConsole);
	cout << "Game Over! Score: " << nScore << endl << endl << endl << endl;
	system("pause");






	return 0;
}