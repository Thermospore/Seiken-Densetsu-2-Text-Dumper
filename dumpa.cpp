#include<stdio.h>

//TO-DO:
//goal: match appearance of in game textbox (bar debug output)
//check rest of stuff in badParses
//finish store/inn/options stuff
//reduce length of address displaying
//fix string coding stuff, so it doesn't take forever to compile...
//in dump, don't base box spacing on <pause>. maybe count rows->3 or something?
//option to A) show exactly like game or B) show special codes too (debug output)
//		maybe can pick between the types. ie textbox start/end, line end, and internal effects
//		easiest to do in post processing, rather than adding an if to every single debug output

//advances to next byte
void advance(unsigned char &cur, FILE* pROM, int &addr)
{
	fread(&cur, 1, 1, pROM); //read next byte
	addr++; //update current address
	return;
}

//Prints game text
void gPrint(char* moji, FILE* pDUMP, int &col)
{
	col++; //advance column
	if (col == 16) //wrap column
	{
		fprintf(pDUMP,"<WRAP>\n");
		col = 0;
	}
	fprintf(pDUMP,"%s",moji); //print char
	return;
}

//Sets column (by spitting out spaces)
void setCol(unsigned char cur, FILE* pDUMP, int &col)
{
	//there are only 16 columns, so anything more is garbage
	if (cur < 0xF)
	{
		int numSpaces = 0;
		
		//to account for wrapping
		if(col == 15)
		{numSpaces = cur;}
		else
		{numSpaces = cur-col-1;}
		
		if(numSpaces > 0)
		{
			while(numSpaces > 0)
			{
				gPrint("�@",pDUMP,col);
				numSpaces--;
			}
		}
		else if(numSpaces < 0)
		{
			fprintf(pDUMP,"<COLNEG%X>",numSpaces*-1);
		}
	}
	return;
}

int main()
{
	unsigned char cur = 0x00; //contents of current byte of the ROM
	char* moji = "��"; //moji to print
	int addr = -1; //current address in the ROM
	int col = -1; //current column of text

	//remove old dump
	remove("dump.txt");
		
	//open files
	FILE* pROM = fopen("Seikendensetsu2.D97C.sfc", "rb");
	FILE* pDUMP = fopen("dump.txt", "w");
	
	//loop through file 1 byte at a time, until end
	while(!feof(pROM))
	{
		advance(cur,pROM,addr);
		
		//end of string
		if(cur == 0x00)
		{
			fprintf(pDUMP,"<EOS:0x%0.6X>\n",addr);
			col = -1;
		}
		//inn price reference?
		else if(cur == 0x02)
		{
			advance(cur,pROM,addr);
			if(cur == 0x00)
			{
				fprintf(pDUMP,"<$>");
			}
			else
			{
				fprintf(pDUMP,"<02?%0.2X>",cur);
			}
		}
		//text stutter
		else if(cur == 0x08)
		{
			fprintf(pDUMP,"<...>");
		}
		//Unknown. Related to attack moves/rendering?
		//Actually, maybe to y/n selections?
		else if(cur == 0x05 || cur == 0x09)
		{
			fprintf(pDUMP,"<%0.2X?>",cur);
		}
		//textbox transition
		else if(cur == 0x28)
		{
			advance(cur,pROM,addr);
			//wait for button press
			if (cur == 0x00)
			{
				fprintf(pDUMP,"<PAUSE>\n\n");
				col = -1;
			}
			//wait for an amount of time
			else
			{
				fprintf(pDUMP,"<WAIT%0.2X>",cur);
			}
		}
		//unknown
		else if(cur == 0x30)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,"<30?%0.2X>",cur);
			advance(cur,pROM,addr);//dump control
		}
		//unknown. attack?
		else if(cur == 0x31)
		{
			advance(cur,pROM,addr);
			if(cur == 0x00)
			{
				//dump control
				advance(cur,pROM,addr);
				fprintf(pDUMP,"<ATK>",cur);
			}
			else
			{
				fprintf(pDUMP,"<31?%0.2X>",cur);	
			}				
		}
		//characchehter movement
		else if(cur == 0x32)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,"<MVE%0.2X>",cur);
			advance(cur,pROM,addr);//dump movement control
		}
		//chahrahrer action?
		else if(cur == 0x34)
		{
			advance(cur,pROM,addr);
			if(cur == 0x00)
			{
				//dump action control
				advance(cur,pROM,addr);
				fprintf(pDUMP,"<ACT>");
			}
			else
			{
				fprintf(pDUMP,"<34?%0.2X>",cur);
			}
		}
		//textbox open
		else if(cur == 0x50)
		{
			fprintf(pDUMP,"\n\n<OPEN:0x%0.6X>\n",addr);
			col = -1;
		}
		//textbox close
		else if(cur == 0x51)
		{
			fprintf(pDUMP,"<CLOSE:0x%0.6X>\n\n",addr);
			col = -1;
		}
		//textbox clear
		else if(cur == 0x52)
		{
			fprintf(pDUMP,"\n\n<CLEAR:0x%0.6X>\n",addr);
			col = -1;
		}
		//charachthtere names
		else if(cur == 0x57)
		{
			advance(cur,pROM,addr);
			if(cur == 0x00)
			{
				fprintf(pDUMP,"<�����f�B>");
				col += 4;
			} 
			else if(cur == 0x01)
			{
				fprintf(pDUMP,"<�v����>");
				col += 3;
			}
			else if(cur == 0x02)
			{
				fprintf(pDUMP,"<�|�|�C>");
				col += 3;
			}
		}
		//initialize a selection list?
		else if(cur == 0x58)
		{
			fprintf(pDUMP,"<SEL>");
		}
		//set current column
		//(this is independent of selection lists)
		else if(cur == 0x59)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,"<COL%X>",cur);
			
			setCol(cur,pDUMP,col);			
		}
		//set the column/location for a selection?
		else if(cur == 0x5A)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,"<SCOL%X>",cur);
			
			setCol(cur,pDUMP,col);	
		}
		//finalize a selection list?
		else if(cur == 0x5B)
		{
			fprintf(pDUMP,"</SEL>\n\n");
		}
		//newline
		else if(cur == 0x7F)
		{
			fprintf(pDUMP,"<NL:0x%0.6X>\n",addr);
			col = -1;
		}
		//no shift
		else if(cur >= 0x80 && cur <= 0xFF)
		{
			switch (cur)
			{
			    case 0x80: moji = "�A";  break;
				case 0x81: moji = "�B";  break;
				case 0x82: moji = "�I";  break;
				case 0x83: moji = "�H";  break;
				case 0x84: moji = "�[";  break;
				case 0x85: moji = "�d";  break;
				case 0x86: moji = "�u";  break;
				case 0x87: moji = "�v";  break;
				case 0x88: moji = "�w";  break;
				case 0x89: moji = "�x";  break;
				case 0x8A: moji = "��";  break;
				case 0x8B: moji = "��";  break;
				case 0x8C: moji = "��";  break;
				case 0x8D: moji = "��";  break;
				case 0x8E: moji = "��";  break;
				case 0x8F: moji = "��";  break;
				case 0x90: moji = "��";  break;
				case 0x91: moji = "��";  break;
				case 0x92: moji = "��";  break;
				case 0x93: moji = "��";  break;
				case 0x94: moji = "��";  break;
				case 0x95: moji = "��";  break;
				case 0x96: moji = "��";  break;
				case 0x97: moji = "��";  break;
				case 0x98: moji = "��";  break;
				case 0x99: moji = "��";  break;
				case 0x9A: moji = "��";  break;
				case 0x9B: moji = "��";  break;
				case 0x9C: moji = "��";  break;
				case 0x9D: moji = "��";  break;
				case 0x9E: moji = "��";  break;
				case 0x9F: moji = "��";  break;
				case 0xA0: moji = "��";  break;
				case 0xA1: moji = "��";  break;
				case 0xA2: moji = "��";  break;
				case 0xA3: moji = "��";  break;
				case 0xA4: moji = "��";  break;
				case 0xA5: moji = "��";  break;
				case 0xA6: moji = "��";  break;
				case 0xA7: moji = "��";  break;
				case 0xA8: moji = "��";  break;
				case 0xA9: moji = "��";  break;
				case 0xAA: moji = "��";  break;
				case 0xAB: moji = "��";  break;
				case 0xAC: moji = "��";  break;
				case 0xAD: moji = "��";  break;
				case 0xAE: moji = "��";  break;
				case 0xAF: moji = "��";  break;
				case 0xB0: moji = "��";  break;
				case 0xB1: moji = "��";  break;
				case 0xB2: moji = "��";  break;
				case 0xB3: moji = "��";  break;
				case 0xB4: moji = "��";  break;
				case 0xB5: moji = "��";  break;
				case 0xB6: moji = "��";  break;
				case 0xB7: moji = "��";  break;
				case 0xB8: moji = "��";  break;
				case 0xB9: moji = "��";  break;
				case 0xBA: moji = "��";  break;
				case 0xBB: moji = "��";  break;
				case 0xBC: moji = "��";  break;
				case 0xBD: moji = "��";  break;
				case 0xBE: moji = "��";  break;
				case 0xBF: moji = "��";  break;
				case 0xC0: moji = "��";  break;
				case 0xC1: moji = "��";  break;
				case 0xC2: moji = "��";  break;
				case 0xC3: moji = "��";  break;
				case 0xC4: moji = "��";  break;
				case 0xC5: moji = "��";  break;
				case 0xC6: moji = "��";  break;
				case 0xC7: moji = "��";  break;
				case 0xC8: moji = "��";  break;
				case 0xC9: moji = "��";  break;
				case 0xCA: moji = "��";  break;
				case 0xCB: moji = "��";  break;
				case 0xCC: moji = "��";  break;
				case 0xCD: moji = "��";  break;
				case 0xCE: moji = "��";  break;
				case 0xCF: moji = "��";  break;
				case 0xD0: moji = "��";  break;
				case 0xD1: moji = "��";  break;
				case 0xD2: moji = "��";  break;
				case 0xD3: moji = "��";  break;
				case 0xD4: moji = "��";  break;
				case 0xD5: moji = "��";  break;
				case 0xD6: moji = "��";  break;
				case 0xD7: moji = "��";  break;
				case 0xD8: moji = "��";  break;
				case 0xD9: moji = "��";  break;
				case 0xDA: moji = "�`";  break;
				case 0xDB: moji = "�a";  break;
				case 0xDC: moji = "�b";  break;
				case 0xDD: moji = "�c";  break;
				case 0xDE: moji = "�d";  break;
				case 0xDF: moji = "�e";  break;
				case 0xE0: moji = "�f";  break;
				case 0xE1: moji = "�g";  break;
				case 0xE2: moji = "�h";  break;
				case 0xE3: moji = "�i";  break;
				case 0xE4: moji = "�j";  break;
				case 0xE5: moji = "�k";  break;
				case 0xE6: moji = "�l";  break;
				case 0xE7: moji = "�m";  break;
				case 0xE8: moji = "�n";  break;
				case 0xE9: moji = "�o";  break;
				case 0xEA: moji = "�p";  break;
				case 0xEB: moji = "�q";  break;
				case 0xEC: moji = "�r";  break;
				case 0xED: moji = "�s";  break;
				case 0xEE: moji = "�t";  break;
				case 0xEF: moji = "�u";  break;
				case 0xF0: moji = "�v";  break;
				case 0xF1: moji = "�w";  break;
				case 0xF2: moji = "�x";  break;
				case 0xF3: moji = "�y";  break;
				case 0xF4: moji = "�E";  break;
				case 0xF5: moji = "�F";  break;
				case 0xF6: moji = "�G";  break;
				case 0xF7: moji = "�`";  break;
				case 0xF8: moji = "�^";  break;
				case 0xF9: moji = "�i";  break;
				case 0xFA: moji = "�j";  break;
				case 0xFB: moji = "��";  break;
				case 0xFC: moji = "��";  break;
				case 0xFD: moji = "�`";  break;
				case 0xFE: moji = "��";  break;
				case 0xFF: moji = "�@";  break;
			}
			gPrint(moji,pDUMP,col);
		}
		//shift 1
		else if(cur >= 0x60 && cur <= 0x67)
		{
			unsigned char shiftLength = (cur-0x60);
			for(int i=0; i <= shiftLength; i++)
			{
				advance(cur,pROM,addr);
				switch (cur)
				{
					case 0x0: moji = "�A";  break;
					case 0x1: moji = "�B";  break;
					case 0x2: moji = "�I";  break;
					case 0x3: moji = "�H";  break;
					case 0x4: moji = "�[";  break;
					case 0x5: moji = "�d";  break;
					case 0x6: moji = "�u";  break;
					case 0x7: moji = "�v";  break;
					case 0x8: moji = "�w";  break;
					case 0x9: moji = "�x";  break;
					case 0x0A: moji = "��";  break;
					case 0x0B: moji = "��";  break;
					case 0x0C: moji = "��";  break;
					case 0x0D: moji = "��";  break;
					case 0x0E: moji = "��";  break;
					case 0x0F: moji = "��";  break;
					case 0x10: moji = "��";  break;
					case 0x11: moji = "��";  break;
					case 0x12: moji = "��";  break;
					case 0x13: moji = "��";  break;
					case 0x14: moji = "��";  break;
					case 0x15: moji = "��";  break;
					case 0x16: moji = "��";  break;
					case 0x17: moji = "��";  break;
					case 0x18: moji = "��";  break;
					case 0x19: moji = "��";  break;
					case 0x1A: moji = "��";  break;
					case 0x1B: moji = "��";  break;
					case 0x1C: moji = "��";  break;
					case 0x1D: moji = "��";  break;
					case 0x1E: moji = "��";  break;
					case 0x1F: moji = "��";  break;
					case 0x20: moji = "��";  break;
					case 0x21: moji = "��";  break;
					case 0x22: moji = "��";  break;
					case 0x23: moji = "��";  break;
					case 0x24: moji = "��";  break;
					case 0x25: moji = "��";  break;
					case 0x26: moji = "��";  break;
					case 0x27: moji = "��";  break;
					case 0x28: moji = "��";  break;
					case 0x29: moji = "��";  break;
					case 0x2A: moji = "��";  break;
					case 0x2B: moji = "��";  break;
					case 0x2C: moji = "��";  break;
					case 0x2D: moji = "��";  break;
					case 0x2E: moji = "��";  break;
					case 0x2F: moji = "��";  break;
					case 0x30: moji = "��";  break;
					case 0x31: moji = "��";  break;
					case 0x32: moji = "��";  break;
					case 0x33: moji = "��";  break;
					case 0x34: moji = "��";  break;
					case 0x35: moji = "��";  break;
					case 0x36: moji = "��";  break;
					case 0x37: moji = "��";  break;
					case 0x38: moji = "��";  break;
					case 0x39: moji = "��";  break;
					case 0x3A: moji = "��";  break;
					case 0x3B: moji = "��";  break;
					case 0x3C: moji = "��";  break;
					case 0x3D: moji = "��";  break;
					case 0x3E: moji = "��";  break;
					case 0x3F: moji = "��";  break;
					case 0x40: moji = "��";  break;
					case 0x41: moji = "��";  break;
					case 0x42: moji = "��";  break;
					case 0x43: moji = "��";  break;
					case 0x44: moji = "��";  break;
					case 0x45: moji = "��";  break;
					case 0x46: moji = "��";  break;
					case 0x47: moji = "��";  break;
					case 0x48: moji = "��";  break;
					case 0x49: moji = "��";  break;
					case 0x4A: moji = "��";  break;
					case 0x4B: moji = "��";  break;
					case 0x4C: moji = "��";  break;
					case 0x4D: moji = "��";  break;
					case 0x4E: moji = "��";  break;
					case 0x4F: moji = "��";  break;
					case 0x50: moji = "��";  break;
					case 0x51: moji = "��";  break;
					case 0x52: moji = "��";  break;
					case 0x53: moji = "��";  break;
					case 0x54: moji = "��";  break;
					case 0x55: moji = "��";  break;
					case 0x56: moji = "��";  break;
					case 0x57: moji = "��";  break;
					case 0x58: moji = "��";  break;
					case 0x59: moji = "��";  break;
					case 0x5A: moji = "�`";  break;
					case 0x5B: moji = "�a";  break;
					case 0x5C: moji = "�b";  break;
					case 0x5D: moji = "�c";  break;
					case 0x5E: moji = "�d";  break;
					case 0x5F: moji = "�e";  break;
					case 0x60: moji = "�f";  break;
					case 0x61: moji = "�g";  break;
					case 0x62: moji = "�h";  break;
					case 0x63: moji = "�i";  break;
					case 0x64: moji = "�j";  break;
					case 0x65: moji = "�k";  break;
					case 0x66: moji = "�l";  break;
					case 0x67: moji = "�m";  break;
					case 0x68: moji = "�n";  break;
					case 0x69: moji = "�o";  break;
					case 0x6A: moji = "�p";  break;
					case 0x6B: moji = "�q";  break;
					case 0x6C: moji = "�r";  break;
					case 0x6D: moji = "�s";  break;
					case 0x6E: moji = "�t";  break;
					case 0x6F: moji = "�u";  break;
					case 0x70: moji = "�v";  break;
					case 0x71: moji = "�w";  break;
					case 0x72: moji = "�x";  break;
					case 0x73: moji = "�y";  break;
					case 0x74: moji = "�E";  break;
					case 0x75: moji = "�F";  break;
					case 0x76: moji = "�G";  break;
					case 0x77: moji = "�`";  break;
					case 0x78: moji = "�^";  break;
					case 0x79: moji = "�i";  break;
					case 0x7A: moji = "�j";  break;
					case 0x7B: moji = "��";  break;
					case 0x7C: moji = "��";  break;
					case 0x7D: moji = "�`";  break;
					case 0x7E: moji = "��";  break;
					case 0x7F: moji = "�@";  break;
					case 0x80: moji = "�O";  break;
					case 0x81: moji = "�P";  break;
					case 0x82: moji = "�Q";  break;
					case 0x83: moji = "�R";  break;
					case 0x84: moji = "�S";  break;
					case 0x85: moji = "�T";  break;
					case 0x86: moji = "�U";  break;
					case 0x87: moji = "�V";  break;
					case 0x88: moji = "�W";  break;
					case 0x89: moji = "�X";  break;
					case 0x8A: moji = "�A";  break;
					case 0x8B: moji = "�C";  break;
					case 0x8C: moji = "�E";  break;
					case 0x8D: moji = "�G";  break;
					case 0x8E: moji = "�I";  break;
					case 0x8F: moji = "�J";  break;
					case 0x90: moji = "�L";  break;
					case 0x91: moji = "�N";  break;
					case 0x92: moji = "�P";  break;
					case 0x93: moji = "�R";  break;
					case 0x94: moji = "�T";  break;
					case 0x95: moji = "�V";  break;
					case 0x96: moji = "�X";  break;
					case 0x97: moji = "�Z";  break;
					case 0x98: moji = "�\\";  break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x99: moji = "�^";  break;
					case 0x9A: moji = "�`";  break;
					case 0x9B: moji = "�c";  break;
					case 0x9C: moji = "�e";  break;
					case 0x9D: moji = "�g";  break;
					case 0x9E: moji = "�i";  break;
					case 0x9F: moji = "�j";  break;
					case 0xA0: moji = "�k";  break;
					case 0xA1: moji = "�l";  break;
					case 0xA2: moji = "�m";  break;
					case 0xA3: moji = "�n";  break;
					case 0xA4: moji = "�q";  break;
					case 0xA5: moji = "�t";  break;
					case 0xA6: moji = "�w";  break;
					case 0xA7: moji = "�z";  break;
					case 0xA8: moji = "�}";  break;
					case 0xA9: moji = "�~";  break;
					case 0xAA: moji = "��";  break;
					case 0xAB: moji = "��";  break;
					case 0xAC: moji = "��";  break;
					case 0xAD: moji = "��";  break;
					case 0xAE: moji = "��";  break;
					case 0xAF: moji = "��";  break;
					case 0xB0: moji = "��";  break;
					case 0xB1: moji = "��";  break;
					case 0xB2: moji = "��";  break;
					case 0xB3: moji = "��";  break;
					case 0xB4: moji = "��";  break;
					case 0xB5: moji = "��";  break;
					case 0xB6: moji = "��";  break;
					case 0xB7: moji = "��";  break;
					case 0xB8: moji = "�b";  break;
					case 0xB9: moji = "��";  break;
					case 0xBA: moji = "��";  break;
					case 0xBB: moji = "��";  break;
					case 0xBC: moji = "�@";  break;
					case 0xBD: moji = "�B";  break;
					case 0xBE: moji = "�D";  break;
					case 0xBF: moji = "�F";  break;
					case 0xC0: moji = "�H";  break;
					case 0xC1: moji = "�K";  break;
					case 0xC2: moji = "�M";  break;
					case 0xC3: moji = "�O";  break;
					case 0xC4: moji = "�Q";  break;
					case 0xC5: moji = "�S";  break;
					case 0xC6: moji = "�U";  break;
					case 0xC7: moji = "�W";  break;
					case 0xC8: moji = "�Y";  break;
					case 0xC9: moji = "�[";  break;
					case 0xCA: moji = "�]";  break;
					case 0xCB: moji = "�_";  break;
					case 0xCC: moji = "�a";  break;
					case 0xCD: moji = "�d";  break;
					case 0xCE: moji = "�f";  break;
					case 0xCF: moji = "�h";  break;
					case 0xD0: moji = "�o";  break;
					case 0xD1: moji = "�r";  break;
					case 0xD2: moji = "�u";  break;
					case 0xD3: moji = "�x";  break;
					case 0xD4: moji = "�{";  break;
					case 0xD5: moji = "�p";  break;
					case 0xD6: moji = "�s";  break;
					case 0xD7: moji = "�v";  break;
					case 0xD8: moji = "�y";  break;
					case 0xD9: moji = "�|";  break;
					case 0xDA: moji = "��";  break;
					case 0xDB: moji = "��";  break;
					case 0xDC: moji = "��";  break;
					case 0xDD: moji = "��";  break;
					case 0xDE: moji = "��";  break;
					case 0xDF: moji = "��";  break;
					case 0xE0: moji = "��";  break;
					case 0xE1: moji = "��";  break;
					case 0xE2: moji = "��";  break;
					case 0xE3: moji = "��";  break;
					case 0xE4: moji = "��";  break;
					case 0xE5: moji = "��";  break;
					case 0xE6: moji = "��";  break;
					case 0xE7: moji = "��";  break;
					case 0xE8: moji = "��";  break;
					case 0xE9: moji = "��";  break;
					case 0xEA: moji = "��";  break;
					case 0xEB: moji = "��";  break;
					case 0xEC: moji = "��";  break;
					case 0xED: moji = "��";  break;
					case 0xEE: moji = "��";  break;
					case 0xEF: moji = "��";  break;
					case 0xF0: moji = "��";  break;
					case 0xF1: moji = "��";  break;
					case 0xF2: moji = "��";  break;
					case 0xF3: moji = "��";  break;
					case 0xF4: moji = "��";  break;
					case 0xF5: moji = "�X";  break;
					case 0xF6: moji = "��";  break;
					case 0xF7: moji = "��";  break;
					case 0xF8: moji = "��";  break;
					case 0xF9: moji = "��";  break;
					case 0xFA: moji = "�p";  break;
					case 0xFB: moji = "��";  break;
					case 0xFC: moji = "��";  break;
					case 0xFD: moji = "�N";  break;
					case 0xFE: moji = "��";  break;
					case 0xFF: moji = "��";  break;
				}
				gPrint(moji,pDUMP,col);
			}
		}
		//shift 2
		else if(cur >= 0x68 && cur <= 0x6B)
		{
			unsigned char shiftLength = (cur-0x68);
			for(int i=0; i <= shiftLength; i++)
			{
				advance(cur,pROM,addr);
				switch (cur)
				{
					case 0x0: moji = "�O";  break;
					case 0x1: moji = "�P";  break;
					case 0x2: moji = "�Q";  break;
					case 0x3: moji = "�R";  break;
					case 0x4: moji = "�S";  break;
					case 0x5: moji = "�T";  break;
					case 0x6: moji = "�U";  break;
					case 0x7: moji = "�V";  break;
					case 0x8: moji = "�W";  break;
					case 0x9: moji = "�X";  break;
					case 0x0A: moji = "�A";  break;
					case 0x0B: moji = "�C";  break;
					case 0x0C: moji = "�E";  break;
					case 0x0D: moji = "�G";  break;
					case 0x0E: moji = "�I";  break;
					case 0x0F: moji = "�J";  break;
					case 0x10: moji = "�L";  break;
					case 0x11: moji = "�N";  break;
					case 0x12: moji = "�P";  break;
					case 0x13: moji = "�R";  break;
					case 0x14: moji = "�T";  break;
					case 0x15: moji = "�V";  break;
					case 0x16: moji = "�X";  break;
					case 0x17: moji = "�Z";  break;
					case 0x18: moji = "�\\";  break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x19: moji = "�^";  break;
					case 0x1A: moji = "�`";  break;
					case 0x1B: moji = "�c";  break;
					case 0x1C: moji = "�e";  break;
					case 0x1D: moji = "�g";  break;
					case 0x1E: moji = "�i";  break;
					case 0x1F: moji = "�j";  break;
					case 0x20: moji = "�k";  break;
					case 0x21: moji = "�l";  break;
					case 0x22: moji = "�m";  break;
					case 0x23: moji = "�n";  break;
					case 0x24: moji = "�q";  break;
					case 0x25: moji = "�t";  break;
					case 0x26: moji = "�w";  break;
					case 0x27: moji = "�z";  break;
					case 0x28: moji = "�}";  break;
					case 0x29: moji = "�~";  break;
					case 0x2A: moji = "��";  break;
					case 0x2B: moji = "��";  break;
					case 0x2C: moji = "��";  break;
					case 0x2D: moji = "��";  break;
					case 0x2E: moji = "��";  break;
					case 0x2F: moji = "��";  break;
					case 0x30: moji = "��";  break;
					case 0x31: moji = "��";  break;
					case 0x32: moji = "��";  break;
					case 0x33: moji = "��";  break;
					case 0x34: moji = "��";  break;
					case 0x35: moji = "��";  break;
					case 0x36: moji = "��";  break;
					case 0x37: moji = "��";  break;
					case 0x38: moji = "�b";  break;
					case 0x39: moji = "��";  break;
					case 0x3A: moji = "��";  break;
					case 0x3B: moji = "��";  break;
					case 0x3C: moji = "�@";  break;
					case 0x3D: moji = "�B";  break;
					case 0x3E: moji = "�D";  break;
					case 0x3F: moji = "�F";  break;
					case 0x40: moji = "�H";  break;
					case 0x41: moji = "�K";  break;
					case 0x42: moji = "�M";  break;
					case 0x43: moji = "�O";  break;
					case 0x44: moji = "�Q";  break;
					case 0x45: moji = "�S";  break;
					case 0x46: moji = "�U";  break;
					case 0x47: moji = "�W";  break;
					case 0x48: moji = "�Y";  break;
					case 0x49: moji = "�[";  break;
					case 0x4A: moji = "�]";  break;
					case 0x4B: moji = "�_";  break;
					case 0x4C: moji = "�a";  break;
					case 0x4D: moji = "�d";  break;
					case 0x4E: moji = "�f";  break;
					case 0x4F: moji = "�h";  break;
					case 0x50: moji = "�o";  break;
					case 0x51: moji = "�r";  break;
					case 0x52: moji = "�u";  break;
					case 0x53: moji = "�x";  break;
					case 0x54: moji = "�{";  break;
					case 0x55: moji = "�p";  break;
					case 0x56: moji = "�s";  break;
					case 0x57: moji = "�v";  break;
					case 0x58: moji = "�y";  break;
					case 0x59: moji = "�|";  break;
					case 0x5A: moji = "��";  break;
					case 0x5B: moji = "��";  break;
					case 0x5C: moji = "��";  break;
					case 0x5D: moji = "��";  break;
					case 0x5E: moji = "��";  break;
					case 0x5F: moji = "��";  break;
					case 0x60: moji = "��";  break;
					case 0x61: moji = "��";  break;
					case 0x62: moji = "��";  break;
					case 0x63: moji = "��";  break;
					case 0x64: moji = "��";  break;
					case 0x65: moji = "��";  break;
					case 0x66: moji = "��";  break;
					case 0x67: moji = "��";  break;
					case 0x68: moji = "��";  break;
					case 0x69: moji = "��";  break;
					case 0x6A: moji = "��";  break;
					case 0x6B: moji = "��";  break;
					case 0x6C: moji = "��";  break;
					case 0x6D: moji = "��";  break;
					case 0x6E: moji = "��";  break;
					case 0x6F: moji = "��";  break;
					case 0x70: moji = "��";  break;
					case 0x71: moji = "��";  break;
					case 0x72: moji = "��";  break;
					case 0x73: moji = "��";  break;
					case 0x74: moji = "��";  break;
					case 0x75: moji = "�X";  break;
					case 0x76: moji = "��";  break;
					case 0x77: moji = "��";  break;
					case 0x78: moji = "��";  break;
					case 0x79: moji = "��";  break;
					case 0x7A: moji = "�p";  break;
					case 0x7B: moji = "��";  break;
					case 0x7C: moji = "��";  break;
					case 0x7D: moji = "�N";  break;
					case 0x7E: moji = "��";  break;
					case 0x7F: moji = "��";  break;
					case 0x80: moji = "��";  break;
					case 0x81: moji = "�l";  break;
					case 0x82: moji = "��";  break;
					case 0x83: moji = "�X";  break;
					case 0x84: moji = "��";  break;
					case 0x85: moji = "��";  break;
					case 0x86: moji = "�v";  break;
					case 0x87: moji = "��";  break;
					case 0x88: moji = "��";  break;
					case 0x89: moji = "��";  break;
					case 0x8A: moji = "�`";  break;
					case 0x8B: moji = "��";  break;
					case 0x8C: moji = "��";  break;
					case 0x8D: moji = "��";  break;
					case 0x8E: moji = "�V";  break;
					case 0x8F: moji = "��";  break;
					case 0x90: moji = "��";  break;
					case 0x91: moji = "��";  break;
					case 0x92: moji = "�j";  break;
					case 0x93: moji = "��";  break;
					case 0x94: moji = "��";  break;
					case 0x95: moji = "�W";  break;
					case 0x96: moji = "�^";  break;
					case 0x97: moji = "�k";  break;
					case 0x98: moji = "��";  break;
					case 0x99: moji = "��";  break;
					case 0x9A: moji = "�n";  break;
					case 0x9B: moji = "��";  break;
					case 0x9C: moji = "��";  break;
					case 0x9D: moji = "��";  break;
					case 0x9E: moji = "��";  break;
					case 0x9F: moji = "��";  break;
					case 0xA0: moji = "�j";  break;
					case 0xA1: moji = "��";  break;
					case 0xA2: moji = "�X";  break;
					case 0xA3: moji = "��";  break;
					case 0xA4: moji = "��";  break;
					case 0xA5: moji = "��";  break;
					case 0xA6: moji = "��";  break;
					case 0xA7: moji = "��";  break;
					case 0xA8: moji = "��";  break;
					case 0xA9: moji = "�q";  break;
					case 0xAA: moji = "��";  break;
					case 0xAB: moji = "�s";  break;
					case 0xAC: moji = "��";  break;
					case 0xAD: moji = "��";  break;
					case 0xAE: moji = "�c";  break;
					case 0xAF: moji = "�v";  break;
					case 0xB0: moji = "��";  break;
					case 0xB1: moji = "�J";  break;
					case 0xB2: moji = "��";  break;
					case 0xB3: moji = "��";  break;
					case 0xB4: moji = "�N";  break;
					case 0xB5: moji = "��";  break;
					case 0xB6: moji = "��";  break;
					case 0xB7: moji = "��";  break;
					case 0xB8: moji = "��";  break;
					case 0xB9: moji = "��";  break;
					case 0xBA: moji = "��";  break;
					case 0xBB: moji = "��";  break;
					case 0xBC: moji = "�R";  break;
					case 0xBD: moji = "�m";  break;
					case 0xBE: moji = "�G";  break;
					case 0xBF: moji = "��";  break;
					case 0xC0: moji = "��";  break;
					case 0xC1: moji = "��";  break;
					case 0xC2: moji = "��";  break;
					case 0xC3: moji = "��";  break;
					case 0xC4: moji = "��";  break;
					case 0xC5: moji = "��";  break;
					case 0xC6: moji = "�v";  break;
					case 0xC7: moji = "��";  break;
					case 0xC8: moji = "��";  break;
					case 0xC9: moji = "��";  break;
					case 0xCA: moji = "��";  break;
					case 0xCB: moji = "��";  break;
					case 0xCC: moji = "�O";  break;
					case 0xCD: moji = "��";  break;
					case 0xCE: moji = "��";  break;
					case 0xCF: moji = "�g";  break;
					case 0xD0: moji = "�{";  break;
					case 0xD1: moji = "��";  break;
					case 0xD2: moji = "��";  break;
					case 0xD3: moji = "�B";  break;
					case 0xD4: moji = "��";  break;
					case 0xD5: moji = "��";  break;
					case 0xD6: moji = "�C";  break;
					case 0xD7: moji = "��";  break;
					case 0xD8: moji = "�b";  break;
					case 0xD9: moji = "�E";  break;
					case 0xDA: moji = "�S";  break;
					case 0xDB: moji = "��";  break;
					case 0xDC: moji = "�D";  break;
					case 0xDD: moji = "��";  break;
					case 0xDE: moji = "��";  break;
					case 0xDF: moji = "��";  break;
					case 0xE0: moji = "��";  break;
					case 0xE1: moji = "��";  break;
					case 0xE2: moji = "��";  break;
					case 0xE3: moji = "�N";  break;
					case 0xE4: moji = "�a";  break;
					case 0xE5: moji = "��";  break;
					case 0xE6: moji = "��";  break;
					case 0xE7: moji = "��";  break;
					case 0xE8: moji = "��";  break;
					case 0xE9: moji = "��";  break;
					case 0xEA: moji = "�n";  break;
					case 0xEB: moji = "��";  break;
					case 0xEC: moji = "��";  break;
					case 0xED: moji = "�_";  break;
					case 0xEE: moji = "��";  break;
					case 0xEF: moji = "��";  break;
					case 0xF0: moji = "�_";  break;
					case 0xF1: moji = "��";  break;
					case 0xF2: moji = "�F";  break;
					case 0xF3: moji = "��";  break;
					case 0xF4: moji = "��";  break;
					case 0xF5: moji = "��";  break;
					case 0xF6: moji = "�R";  break;
					case 0xF7: moji = "�w";  break;
					case 0xF8: moji = "��";  break;
					case 0xF9: moji = "��";  break;
					case 0xFA: moji = "��";  break;
					case 0xFB: moji = "MP";  break;
					case 0xFC: moji = "��";  break;
					case 0xFD: moji = "��";  break;
					case 0xFE: moji = "��";  break;
					case 0xFF: moji = "��";  break;
				}
				gPrint(moji,pDUMP,col);
			}
		}
		//shift 3
		else if(cur >= 0x6C && cur <= 0x6F)
		{
			unsigned char shiftLength = (cur-0x6C);
			for(int i=0; i <= shiftLength; i++)
			{
				advance(cur,pROM,addr);
				switch (cur)
				{
					case 0x0: moji = "��";  break;
					case 0x1: moji = "�l";  break;
					case 0x2: moji = "��";  break;
					case 0x3: moji = "�X";  break;
					case 0x4: moji = "��";  break;
					case 0x5: moji = "��";  break;
					case 0x6: moji = "�v";  break;
					case 0x7: moji = "��";  break;
					case 0x8: moji = "��";  break;
					case 0x9: moji = "��";  break;
					case 0x0A: moji = "�`";  break;
					case 0x0B: moji = "��";  break;
					case 0x0C: moji = "��";  break;
					case 0x0D: moji = "��";  break;
					case 0x0E: moji = "�V";  break;
					case 0x0F: moji = "��";  break;
					case 0x10: moji = "��";  break;
					case 0x11: moji = "��";  break;
					case 0x12: moji = "�j";  break;
					case 0x13: moji = "��";  break;
					case 0x14: moji = "��";  break;
					case 0x15: moji = "�W";  break;
					case 0x16: moji = "�^";  break;
					case 0x17: moji = "�k";  break;
					case 0x18: moji = "��";  break;
					case 0x19: moji = "��";  break;
					case 0x1A: moji = "�n";  break;
					case 0x1B: moji = "��";  break;
					case 0x1C: moji = "��";  break;
					case 0x1D: moji = "��";  break;
					case 0x1E: moji = "��";  break;
					case 0x1F: moji = "��";  break;
					case 0x20: moji = "�j";  break;
					case 0x21: moji = "��";  break;
					case 0x22: moji = "�X";  break;
					case 0x23: moji = "��";  break;
					case 0x24: moji = "��";  break;
					case 0x25: moji = "��";  break;
					case 0x26: moji = "��";  break;
					case 0x27: moji = "��";  break;
					case 0x28: moji = "��";  break;
					case 0x29: moji = "�q";  break;
					case 0x2A: moji = "��";  break;
					case 0x2B: moji = "�s";  break;
					case 0x2C: moji = "��";  break;
					case 0x2D: moji = "��";  break;
					case 0x2E: moji = "�c";  break;
					case 0x2F: moji = "�v";  break;
					case 0x30: moji = "��";  break;
					case 0x31: moji = "�J";  break;
					case 0x32: moji = "��";  break;
					case 0x33: moji = "��";  break;
					case 0x34: moji = "�N";  break;
					case 0x35: moji = "��";  break;
					case 0x36: moji = "��";  break;
					case 0x37: moji = "��";  break;
					case 0x38: moji = "��";  break;
					case 0x39: moji = "��";  break;
					case 0x3A: moji = "��";  break;
					case 0x3B: moji = "��";  break;
					case 0x3C: moji = "�R";  break;
					case 0x3D: moji = "�m";  break;
					case 0x3E: moji = "�G";  break;
					case 0x3F: moji = "��";  break;
					case 0x40: moji = "��";  break;
					case 0x41: moji = "��";  break;
					case 0x42: moji = "��";  break;
					case 0x43: moji = "��";  break;
					case 0x44: moji = "��";  break;
					case 0x45: moji = "��";  break;
					case 0x46: moji = "�v";  break;
					case 0x47: moji = "��";  break;
					case 0x48: moji = "��";  break;
					case 0x49: moji = "��";  break;
					case 0x4A: moji = "��";  break;
					case 0x4B: moji = "��";  break;
					case 0x4C: moji = "�O";  break;
					case 0x4D: moji = "��";  break;
					case 0x4E: moji = "��";  break;
					case 0x4F: moji = "�g";  break;
					case 0x50: moji = "�{";  break;
					case 0x51: moji = "��";  break;
					case 0x52: moji = "��";  break;
					case 0x53: moji = "�B";  break;
					case 0x54: moji = "��";  break;
					case 0x55: moji = "��";  break;
					case 0x56: moji = "�C";  break;
					case 0x57: moji = "��";  break;
					case 0x58: moji = "�b";  break;
					case 0x59: moji = "�E";  break;
					case 0x5A: moji = "�S";  break;
					case 0x5B: moji = "��";  break;
					case 0x5C: moji = "�D";  break;
					case 0x5D: moji = "��";  break;
					case 0x5E: moji = "��";  break;
					case 0x5F: moji = "��";  break;
					case 0x60: moji = "��";  break;
					case 0x61: moji = "��";  break;
					case 0x62: moji = "��";  break;
					case 0x63: moji = "�N";  break;
					case 0x64: moji = "�a";  break;
					case 0x65: moji = "��";  break;
					case 0x66: moji = "��";  break;
					case 0x67: moji = "��";  break;
					case 0x68: moji = "��";  break;
					case 0x69: moji = "��";  break;
					case 0x6A: moji = "�n";  break;
					case 0x6B: moji = "��";  break;
					case 0x6C: moji = "��";  break;
					case 0x6D: moji = "�_";  break;
					case 0x6E: moji = "��";  break;
					case 0x6F: moji = "��";  break;
					case 0x70: moji = "�_";  break;
					case 0x71: moji = "��";  break;
					case 0x72: moji = "�F";  break;
					case 0x73: moji = "��";  break;
					case 0x74: moji = "��";  break;
					case 0x75: moji = "��";  break;
					case 0x76: moji = "�R";  break;
					case 0x77: moji = "�w";  break;
					case 0x78: moji = "��";  break;
					case 0x79: moji = "��";  break;
					case 0x7A: moji = "��";  break;
					case 0x7B: moji = "MP";  break;
					case 0x7C: moji = "��";  break;
					case 0x7D: moji = "��";  break;
					case 0x7E: moji = "��";  break;
					case 0x7F: moji = "��";  break;
					case 0x80: moji = "��";  break;
					case 0x81: moji = "��";  break;
					case 0x82: moji = "��";  break;
					case 0x83: moji = "��";  break;
					case 0x84: moji = "�\\";  break;//more wierdness
					case 0x85: moji = "��";  break;
					case 0x86: moji = "�I";  break;
					case 0x87: moji = "��";  break;
					case 0x88: moji = "��";  break;
					case 0x89: moji = "��";  break;
					case 0x8A: moji = "�E";  break;
					case 0x8B: moji = "�i";  break;
					case 0x8C: moji = "��";  break;
					case 0x8D: moji = "��";  break;
					case 0x8E: moji = "��";  break;
					case 0x8F: moji = "��";  break;
					case 0x90: moji = "�@";  break;
					case 0x91: moji = "��";  break;
					case 0x92: moji = "�l";  break;
					case 0x93: moji = "��";  break;
					case 0x94: moji = "��";  break;
					case 0x95: moji = "��";  break;
					case 0x96: moji = "��";  break;
					case 0x97: moji = "�e";  break;
					case 0x98: moji = "�U";  break;
					case 0x99: moji = "�n";  break;
					case 0x9A: moji = "��";  break;
					case 0x9B: moji = "��";  break;
					case 0x9C: moji = "��";  break;
					case 0x9D: moji = "�I";  break;
					case 0x9E: moji = "�h";  break;
					case 0x9F: moji = "��";  break;
					case 0xA0: moji = "��";  break;
					case 0xA1: moji = "��";  break;
					case 0xA2: moji = "��";  break;
					case 0xA3: moji = "��";  break;
					case 0xA4: moji = "��";  break;
					case 0xA5: moji = "��";  break;
					case 0xA6: moji = "��";  break;
					case 0xA7: moji = "��";  break;
					case 0xA8: moji = "�Z";  break;
					case 0xA9: moji = "��";  break;
					case 0xAA: moji = "��";  break;
					case 0xAB: moji = "��";  break;
					case 0xAC: moji = "��";  break;
					case 0xAD: moji = "��";  break;
					case 0xAE: moji = "��";  break;
					case 0xAF: moji = "�a";  break;
					case 0xB0: moji = "�m";  break;
					case 0xB1: moji = "��";  break;
					case 0xB2: moji = "�X";  break;
					case 0xB3: moji = "�D";  break;
					case 0xB4: moji = "��";  break;
					case 0xB5: moji = "�_";  break;
					case 0xB6: moji = "��";  break;
					case 0xB7: moji = "�O";  break;
					case 0xB8: moji = "��";  break;
					case 0xB9: moji = "��";  break;
					case 0xBA: moji = "��";  break;
					case 0xBB: moji = "��";  break;
					case 0xBC: moji = "��";  break;
					case 0xBD: moji = "�E";  break;
					case 0xBE: moji = "�b";  break;
					case 0xBF: moji = "��";  break;
					case 0xC0: moji = "�@";  break;
					case 0xC1: moji = "��";  break;
					case 0xC2: moji = "��";  break;
					case 0xC3: moji = "��";  break;
					case 0xC4: moji = "��";  break;
					case 0xC5: moji = "�E";  break;
					case 0xC6: moji = "�C";  break;
					case 0xC7: moji = "��";  break;
					case 0xC8: moji = "�b";  break;
					case 0xC9: moji = "�S";  break;
					case 0xCA: moji = "��";  break;
					case 0xCB: moji = "��";  break;
					case 0xCC: moji = "��";  break;
					case 0xCD: moji = "��";  break;
					case 0xCE: moji = "�U";  break;
					case 0xCF: moji = "�N";  break;
					case 0xD0: moji = "��";  break;
					case 0xD1: moji = "��";  break;
					case 0xD2: moji = "��";  break;
					case 0xD3: moji = "��";  break;
					case 0xD4: moji = "�V";  break;
					case 0xD5: moji = "��";  break;
					case 0xD6: moji = "��";  break;
					case 0xD7: moji = "�g";  break;
					case 0xD8: moji = "��";  break;
					case 0xD9: moji = "��";  break;
					case 0xDA: moji = "��";  break;
					case 0xDB: moji = "�v";  break;
					case 0xDC: moji = "��";  break;
					case 0xDD: moji = "��";  break;
					case 0xDE: moji = "��";  break;
					case 0xDF: moji = "�H";  break;
					case 0xE0: moji = "��";  break;
					case 0xE1: moji = "��";  break;
					case 0xE2: moji = "�o";  break;
					case 0xE3: moji = "��";  break;
					case 0xE4: moji = "�q";  break;
					case 0xE5: moji = "�C";  break;
					case 0xE6: moji = "��";  break;
					case 0xE7: moji = "�e";  break;
					case 0xE8: moji = "�j";  break;
					case 0xE9: moji = "�t";  break;
					case 0xEA: moji = "��";  break;
					case 0xEB: moji = "��";  break;
					case 0xEC: moji = "��";  break;
					case 0xED: moji = "��";  break;
					case 0xEE: moji = "��";  break;
					case 0xEF: moji = "��";  break;
					case 0xF0: moji = "��";  break;
					case 0xF1: moji = "��";  break;
					case 0xF2: moji = "��";  break;
					case 0xF3: moji = "��";  break;
					case 0xF4: moji = "��";  break;
					case 0xF5: moji = "��";  break;
					case 0xF6: moji = "��";  break;
					case 0xF7: moji = "�|";  break;
					case 0xF8: moji = "�S";  break;
					case 0xF9: moji = "�\\";  break;//more wierdness
					case 0xFA: moji = "��";  break;
					case 0xFB: moji = "�d";  break;
					case 0xFC: moji = "�C";  break;
					case 0xFD: moji = "��";  break;
					case 0xFE: moji = "��";  break;
					case 0xFF: moji = "��";  break;
				}
				gPrint(moji,pDUMP,col);
			}
		}
	}
	
	//denote end of file
	fprintf(pDUMP,"<END>");
	
	//close files
	fclose(pROM);
	fclose(pDUMP);
	
	return 0;
}
