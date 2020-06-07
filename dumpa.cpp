#include<stdio.h>

//TO-DO:
//goal: match appearance of in game textbox
//identify true start and end of text boxes to phase out 0x00s as start
//streamline output and address display
//account for special text effects, like pauses
//identify autoscroll and buttongated textboxes?

//advances to next byte
void advance(unsigned char &cur, FILE* pROM, int &addr)
{
	fread(&cur, 1, 1, pROM); //read next byte
	addr++; //update current address
	return;
}

int main()
{
	unsigned char cur = 0x00; //contents of current byte of the ROM
	int addr = -1; //current address in the ROM
	bool newString = true; //denotes start of new string (for address display)
	int col = -1; //denotes current col of printed text

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
		if(cur == 0x00 && newString == false)
		{
			fprintf(pDUMP,"<EOS>\n");
			newString = true; //so address will be printed next loop
			col = -1; //reset column
		}
		//charachthtere names
		else if(cur == 0x57)
		{
			advance(cur,pROM,addr);
			if(newString){fprintf(pDUMP,"\n<%0.6X>\n",addr-1); newString = false;} //address display
				
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
		//newline
		else if(cur == 0x7F)
		{
			fprintf(pDUMP,"<NL>\n");
			col = -1; //reset column
		}
		//no shift
		else if(cur >= 0x80 && cur <= 0xFF)
		{
			if(newString){fprintf(pDUMP,"\n<%0.6X>\n",addr); newString = false;} //address display
			
			col++; // advance current col
			if (col == 16)
			{
				col = 0;
				fprintf(pDUMP,"<WRAP>\n");
			}
			
			switch (cur)
			{
			    case 0x80: fprintf(pDUMP,"�A"); break;
				case 0x81: fprintf(pDUMP,"�B"); break;
				case 0x82: fprintf(pDUMP,"�I"); break;
				case 0x83: fprintf(pDUMP,"�H"); break;
				case 0x84: fprintf(pDUMP,"�["); break;
				case 0x85: fprintf(pDUMP,"�d"); break;
				case 0x86: fprintf(pDUMP,"�u"); break;
				case 0x87: fprintf(pDUMP,"�v"); break;
				case 0x88: fprintf(pDUMP,"�w"); break;
				case 0x89: fprintf(pDUMP,"�x"); break;
				case 0x8A: fprintf(pDUMP,"��"); break;
				case 0x8B: fprintf(pDUMP,"��"); break;
				case 0x8C: fprintf(pDUMP,"��"); break;
				case 0x8D: fprintf(pDUMP,"��"); break;
				case 0x8E: fprintf(pDUMP,"��"); break;
				case 0x8F: fprintf(pDUMP,"��"); break;
				case 0x90: fprintf(pDUMP,"��"); break;
				case 0x91: fprintf(pDUMP,"��"); break;
				case 0x92: fprintf(pDUMP,"��"); break;
				case 0x93: fprintf(pDUMP,"��"); break;
				case 0x94: fprintf(pDUMP,"��"); break;
				case 0x95: fprintf(pDUMP,"��"); break;
				case 0x96: fprintf(pDUMP,"��"); break;
				case 0x97: fprintf(pDUMP,"��"); break;
				case 0x98: fprintf(pDUMP,"��"); break;
				case 0x99: fprintf(pDUMP,"��"); break;
				case 0x9A: fprintf(pDUMP,"��"); break;
				case 0x9B: fprintf(pDUMP,"��"); break;
				case 0x9C: fprintf(pDUMP,"��"); break;
				case 0x9D: fprintf(pDUMP,"��"); break;
				case 0x9E: fprintf(pDUMP,"��"); break;
				case 0x9F: fprintf(pDUMP,"��"); break;
				case 0xA0: fprintf(pDUMP,"��"); break;
				case 0xA1: fprintf(pDUMP,"��"); break;
				case 0xA2: fprintf(pDUMP,"��"); break;
				case 0xA3: fprintf(pDUMP,"��"); break;
				case 0xA4: fprintf(pDUMP,"��"); break;
				case 0xA5: fprintf(pDUMP,"��"); break;
				case 0xA6: fprintf(pDUMP,"��"); break;
				case 0xA7: fprintf(pDUMP,"��"); break;
				case 0xA8: fprintf(pDUMP,"��"); break;
				case 0xA9: fprintf(pDUMP,"��"); break;
				case 0xAA: fprintf(pDUMP,"��"); break;
				case 0xAB: fprintf(pDUMP,"��"); break;
				case 0xAC: fprintf(pDUMP,"��"); break;
				case 0xAD: fprintf(pDUMP,"��"); break;
				case 0xAE: fprintf(pDUMP,"��"); break;
				case 0xAF: fprintf(pDUMP,"��"); break;
				case 0xB0: fprintf(pDUMP,"��"); break;
				case 0xB1: fprintf(pDUMP,"��"); break;
				case 0xB2: fprintf(pDUMP,"��"); break;
				case 0xB3: fprintf(pDUMP,"��"); break;
				case 0xB4: fprintf(pDUMP,"��"); break;
				case 0xB5: fprintf(pDUMP,"��"); break;
				case 0xB6: fprintf(pDUMP,"��"); break;
				case 0xB7: fprintf(pDUMP,"��"); break;
				case 0xB8: fprintf(pDUMP,"��"); break;
				case 0xB9: fprintf(pDUMP,"��"); break;
				case 0xBA: fprintf(pDUMP,"��"); break;
				case 0xBB: fprintf(pDUMP,"��"); break;
				case 0xBC: fprintf(pDUMP,"��"); break;
				case 0xBD: fprintf(pDUMP,"��"); break;
				case 0xBE: fprintf(pDUMP,"��"); break;
				case 0xBF: fprintf(pDUMP,"��"); break;
				case 0xC0: fprintf(pDUMP,"��"); break;
				case 0xC1: fprintf(pDUMP,"��"); break;
				case 0xC2: fprintf(pDUMP,"��"); break;
				case 0xC3: fprintf(pDUMP,"��"); break;
				case 0xC4: fprintf(pDUMP,"��"); break;
				case 0xC5: fprintf(pDUMP,"��"); break;
				case 0xC6: fprintf(pDUMP,"��"); break;
				case 0xC7: fprintf(pDUMP,"��"); break;
				case 0xC8: fprintf(pDUMP,"��"); break;
				case 0xC9: fprintf(pDUMP,"��"); break;
				case 0xCA: fprintf(pDUMP,"��"); break;
				case 0xCB: fprintf(pDUMP,"��"); break;
				case 0xCC: fprintf(pDUMP,"��"); break;
				case 0xCD: fprintf(pDUMP,"��"); break;
				case 0xCE: fprintf(pDUMP,"��"); break;
				case 0xCF: fprintf(pDUMP,"��"); break;
				case 0xD0: fprintf(pDUMP,"��"); break;
				case 0xD1: fprintf(pDUMP,"��"); break;
				case 0xD2: fprintf(pDUMP,"��"); break;
				case 0xD3: fprintf(pDUMP,"��"); break;
				case 0xD4: fprintf(pDUMP,"��"); break;
				case 0xD5: fprintf(pDUMP,"��"); break;
				case 0xD6: fprintf(pDUMP,"��"); break;
				case 0xD7: fprintf(pDUMP,"��"); break;
				case 0xD8: fprintf(pDUMP,"��"); break;
				case 0xD9: fprintf(pDUMP,"��"); break;
				case 0xDA: fprintf(pDUMP,"�`"); break;
				case 0xDB: fprintf(pDUMP,"�a"); break;
				case 0xDC: fprintf(pDUMP,"�b"); break;
				case 0xDD: fprintf(pDUMP,"�c"); break;
				case 0xDE: fprintf(pDUMP,"�d"); break;
				case 0xDF: fprintf(pDUMP,"�e"); break;
				case 0xE0: fprintf(pDUMP,"�f"); break;
				case 0xE1: fprintf(pDUMP,"�g"); break;
				case 0xE2: fprintf(pDUMP,"�h"); break;
				case 0xE3: fprintf(pDUMP,"�i"); break;
				case 0xE4: fprintf(pDUMP,"�j"); break;
				case 0xE5: fprintf(pDUMP,"�k"); break;
				case 0xE6: fprintf(pDUMP,"�l"); break;
				case 0xE7: fprintf(pDUMP,"�m"); break;
				case 0xE8: fprintf(pDUMP,"�n"); break;
				case 0xE9: fprintf(pDUMP,"�o"); break;
				case 0xEA: fprintf(pDUMP,"�p"); break;
				case 0xEB: fprintf(pDUMP,"�q"); break;
				case 0xEC: fprintf(pDUMP,"�r"); break;
				case 0xED: fprintf(pDUMP,"�s"); break;
				case 0xEE: fprintf(pDUMP,"�t"); break;
				case 0xEF: fprintf(pDUMP,"�u"); break;
				case 0xF0: fprintf(pDUMP,"�v"); break;
				case 0xF1: fprintf(pDUMP,"�w"); break;
				case 0xF2: fprintf(pDUMP,"�x"); break;
				case 0xF3: fprintf(pDUMP,"�y"); break;
				case 0xF4: fprintf(pDUMP,"�E"); break;
				case 0xF5: fprintf(pDUMP,"�F"); break;
				case 0xF6: fprintf(pDUMP,"�G"); break;
				case 0xF7: fprintf(pDUMP,"�`"); break;
				case 0xF8: fprintf(pDUMP,"�^"); break;
				case 0xF9: fprintf(pDUMP,"�i"); break;
				case 0xFA: fprintf(pDUMP,"�j"); break;
				case 0xFB: fprintf(pDUMP,"��"); break;
				case 0xFC: fprintf(pDUMP,"��"); break;
				case 0xFD: fprintf(pDUMP,"�`"); break;
				case 0xFE: fprintf(pDUMP,"��"); break;
				case 0xFF: fprintf(pDUMP,"�@"); break;
			}
		}
		//shift 1
		else if(cur >= 0x60 && cur <= 0x67)
		{
			if(newString){fprintf(pDUMP,"\n<%0.6X>\n",addr); newString = false;}
			
			unsigned char shiftLength = (cur-0x60);
			for(int i=0; i <= shiftLength; i++)
			{
				advance(cur,pROM,addr);
				
				col++; // advance current col
				if (col == 16)
				{
					col = 0;
					fprintf(pDUMP,"<WRAP>\n");
				}
				
				switch (cur)
				{
					case 0x0: fprintf(pDUMP,"�A"); break;
					case 0x1: fprintf(pDUMP,"�B"); break;
					case 0x2: fprintf(pDUMP,"�I"); break;
					case 0x3: fprintf(pDUMP,"�H"); break;
					case 0x4: fprintf(pDUMP,"�["); break;
					case 0x5: fprintf(pDUMP,"�d"); break;
					case 0x6: fprintf(pDUMP,"�u"); break;
					case 0x7: fprintf(pDUMP,"�v"); break;
					case 0x8: fprintf(pDUMP,"�w"); break;
					case 0x9: fprintf(pDUMP,"�x"); break;
					case 0x0A: fprintf(pDUMP,"��"); break;
					case 0x0B: fprintf(pDUMP,"��"); break;
					case 0x0C: fprintf(pDUMP,"��"); break;
					case 0x0D: fprintf(pDUMP,"��"); break;
					case 0x0E: fprintf(pDUMP,"��"); break;
					case 0x0F: fprintf(pDUMP,"��"); break;
					case 0x10: fprintf(pDUMP,"��"); break;
					case 0x11: fprintf(pDUMP,"��"); break;
					case 0x12: fprintf(pDUMP,"��"); break;
					case 0x13: fprintf(pDUMP,"��"); break;
					case 0x14: fprintf(pDUMP,"��"); break;
					case 0x15: fprintf(pDUMP,"��"); break;
					case 0x16: fprintf(pDUMP,"��"); break;
					case 0x17: fprintf(pDUMP,"��"); break;
					case 0x18: fprintf(pDUMP,"��"); break;
					case 0x19: fprintf(pDUMP,"��"); break;
					case 0x1A: fprintf(pDUMP,"��"); break;
					case 0x1B: fprintf(pDUMP,"��"); break;
					case 0x1C: fprintf(pDUMP,"��"); break;
					case 0x1D: fprintf(pDUMP,"��"); break;
					case 0x1E: fprintf(pDUMP,"��"); break;
					case 0x1F: fprintf(pDUMP,"��"); break;
					case 0x20: fprintf(pDUMP,"��"); break;
					case 0x21: fprintf(pDUMP,"��"); break;
					case 0x22: fprintf(pDUMP,"��"); break;
					case 0x23: fprintf(pDUMP,"��"); break;
					case 0x24: fprintf(pDUMP,"��"); break;
					case 0x25: fprintf(pDUMP,"��"); break;
					case 0x26: fprintf(pDUMP,"��"); break;
					case 0x27: fprintf(pDUMP,"��"); break;
					case 0x28: fprintf(pDUMP,"��"); break;
					case 0x29: fprintf(pDUMP,"��"); break;
					case 0x2A: fprintf(pDUMP,"��"); break;
					case 0x2B: fprintf(pDUMP,"��"); break;
					case 0x2C: fprintf(pDUMP,"��"); break;
					case 0x2D: fprintf(pDUMP,"��"); break;
					case 0x2E: fprintf(pDUMP,"��"); break;
					case 0x2F: fprintf(pDUMP,"��"); break;
					case 0x30: fprintf(pDUMP,"��"); break;
					case 0x31: fprintf(pDUMP,"��"); break;
					case 0x32: fprintf(pDUMP,"��"); break;
					case 0x33: fprintf(pDUMP,"��"); break;
					case 0x34: fprintf(pDUMP,"��"); break;
					case 0x35: fprintf(pDUMP,"��"); break;
					case 0x36: fprintf(pDUMP,"��"); break;
					case 0x37: fprintf(pDUMP,"��"); break;
					case 0x38: fprintf(pDUMP,"��"); break;
					case 0x39: fprintf(pDUMP,"��"); break;
					case 0x3A: fprintf(pDUMP,"��"); break;
					case 0x3B: fprintf(pDUMP,"��"); break;
					case 0x3C: fprintf(pDUMP,"��"); break;
					case 0x3D: fprintf(pDUMP,"��"); break;
					case 0x3E: fprintf(pDUMP,"��"); break;
					case 0x3F: fprintf(pDUMP,"��"); break;
					case 0x40: fprintf(pDUMP,"��"); break;
					case 0x41: fprintf(pDUMP,"��"); break;
					case 0x42: fprintf(pDUMP,"��"); break;
					case 0x43: fprintf(pDUMP,"��"); break;
					case 0x44: fprintf(pDUMP,"��"); break;
					case 0x45: fprintf(pDUMP,"��"); break;
					case 0x46: fprintf(pDUMP,"��"); break;
					case 0x47: fprintf(pDUMP,"��"); break;
					case 0x48: fprintf(pDUMP,"��"); break;
					case 0x49: fprintf(pDUMP,"��"); break;
					case 0x4A: fprintf(pDUMP,"��"); break;
					case 0x4B: fprintf(pDUMP,"��"); break;
					case 0x4C: fprintf(pDUMP,"��"); break;
					case 0x4D: fprintf(pDUMP,"��"); break;
					case 0x4E: fprintf(pDUMP,"��"); break;
					case 0x4F: fprintf(pDUMP,"��"); break;
					case 0x50: fprintf(pDUMP,"��"); break;
					case 0x51: fprintf(pDUMP,"��"); break;
					case 0x52: fprintf(pDUMP,"��"); break;
					case 0x53: fprintf(pDUMP,"��"); break;
					case 0x54: fprintf(pDUMP,"��"); break;
					case 0x55: fprintf(pDUMP,"��"); break;
					case 0x56: fprintf(pDUMP,"��"); break;
					case 0x57: fprintf(pDUMP,"��"); break;
					case 0x58: fprintf(pDUMP,"��"); break;
					case 0x59: fprintf(pDUMP,"��"); break;
					case 0x5A: fprintf(pDUMP,"�`"); break;
					case 0x5B: fprintf(pDUMP,"�a"); break;
					case 0x5C: fprintf(pDUMP,"�b"); break;
					case 0x5D: fprintf(pDUMP,"�c"); break;
					case 0x5E: fprintf(pDUMP,"�d"); break;
					case 0x5F: fprintf(pDUMP,"�e"); break;
					case 0x60: fprintf(pDUMP,"�f"); break;
					case 0x61: fprintf(pDUMP,"�g"); break;
					case 0x62: fprintf(pDUMP,"�h"); break;
					case 0x63: fprintf(pDUMP,"�i"); break;
					case 0x64: fprintf(pDUMP,"�j"); break;
					case 0x65: fprintf(pDUMP,"�k"); break;
					case 0x66: fprintf(pDUMP,"�l"); break;
					case 0x67: fprintf(pDUMP,"�m"); break;
					case 0x68: fprintf(pDUMP,"�n"); break;
					case 0x69: fprintf(pDUMP,"�o"); break;
					case 0x6A: fprintf(pDUMP,"�p"); break;
					case 0x6B: fprintf(pDUMP,"�q"); break;
					case 0x6C: fprintf(pDUMP,"�r"); break;
					case 0x6D: fprintf(pDUMP,"�s"); break;
					case 0x6E: fprintf(pDUMP,"�t"); break;
					case 0x6F: fprintf(pDUMP,"�u"); break;
					case 0x70: fprintf(pDUMP,"�v"); break;
					case 0x71: fprintf(pDUMP,"�w"); break;
					case 0x72: fprintf(pDUMP,"�x"); break;
					case 0x73: fprintf(pDUMP,"�y"); break;
					case 0x74: fprintf(pDUMP,"�E"); break;
					case 0x75: fprintf(pDUMP,"�F"); break;
					case 0x76: fprintf(pDUMP,"�G"); break;
					case 0x77: fprintf(pDUMP,"�`"); break;
					case 0x78: fprintf(pDUMP,"�^"); break;
					case 0x79: fprintf(pDUMP,"�i"); break;
					case 0x7A: fprintf(pDUMP,"�j"); break;
					case 0x7B: fprintf(pDUMP,"��"); break;
					case 0x7C: fprintf(pDUMP,"��"); break;
					case 0x7D: fprintf(pDUMP,"�`"); break;
					case 0x7E: fprintf(pDUMP,"��"); break;
					case 0x7F: fprintf(pDUMP,"�@"); break;
					case 0x80: fprintf(pDUMP,"�O"); break;
					case 0x81: fprintf(pDUMP,"�P"); break;
					case 0x82: fprintf(pDUMP,"�Q"); break;
					case 0x83: fprintf(pDUMP,"�R"); break;
					case 0x84: fprintf(pDUMP,"�S"); break;
					case 0x85: fprintf(pDUMP,"�T"); break;
					case 0x86: fprintf(pDUMP,"�U"); break;
					case 0x87: fprintf(pDUMP,"�V"); break;
					case 0x88: fprintf(pDUMP,"�W"); break;
					case 0x89: fprintf(pDUMP,"�X"); break;
					case 0x8A: fprintf(pDUMP,"�A"); break;
					case 0x8B: fprintf(pDUMP,"�C"); break;
					case 0x8C: fprintf(pDUMP,"�E"); break;
					case 0x8D: fprintf(pDUMP,"�G"); break;
					case 0x8E: fprintf(pDUMP,"�I"); break;
					case 0x8F: fprintf(pDUMP,"�J"); break;
					case 0x90: fprintf(pDUMP,"�L"); break;
					case 0x91: fprintf(pDUMP,"�N"); break;
					case 0x92: fprintf(pDUMP,"�P"); break;
					case 0x93: fprintf(pDUMP,"�R"); break;
					case 0x94: fprintf(pDUMP,"�T"); break;
					case 0x95: fprintf(pDUMP,"�V"); break;
					case 0x96: fprintf(pDUMP,"�X"); break;
					case 0x97: fprintf(pDUMP,"�Z"); break;
					case 0x98: fprintf(pDUMP,"�\\"); break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x99: fprintf(pDUMP,"�^"); break;
					case 0x9A: fprintf(pDUMP,"�`"); break;
					case 0x9B: fprintf(pDUMP,"�c"); break;
					case 0x9C: fprintf(pDUMP,"�e"); break;
					case 0x9D: fprintf(pDUMP,"�g"); break;
					case 0x9E: fprintf(pDUMP,"�i"); break;
					case 0x9F: fprintf(pDUMP,"�j"); break;
					case 0xA0: fprintf(pDUMP,"�k"); break;
					case 0xA1: fprintf(pDUMP,"�l"); break;
					case 0xA2: fprintf(pDUMP,"�m"); break;
					case 0xA3: fprintf(pDUMP,"�n"); break;
					case 0xA4: fprintf(pDUMP,"�q"); break;
					case 0xA5: fprintf(pDUMP,"�t"); break;
					case 0xA6: fprintf(pDUMP,"�w"); break;
					case 0xA7: fprintf(pDUMP,"�z"); break;
					case 0xA8: fprintf(pDUMP,"�}"); break;
					case 0xA9: fprintf(pDUMP,"�~"); break;
					case 0xAA: fprintf(pDUMP,"��"); break;
					case 0xAB: fprintf(pDUMP,"��"); break;
					case 0xAC: fprintf(pDUMP,"��"); break;
					case 0xAD: fprintf(pDUMP,"��"); break;
					case 0xAE: fprintf(pDUMP,"��"); break;
					case 0xAF: fprintf(pDUMP,"��"); break;
					case 0xB0: fprintf(pDUMP,"��"); break;
					case 0xB1: fprintf(pDUMP,"��"); break;
					case 0xB2: fprintf(pDUMP,"��"); break;
					case 0xB3: fprintf(pDUMP,"��"); break;
					case 0xB4: fprintf(pDUMP,"��"); break;
					case 0xB5: fprintf(pDUMP,"��"); break;
					case 0xB6: fprintf(pDUMP,"��"); break;
					case 0xB7: fprintf(pDUMP,"��"); break;
					case 0xB8: fprintf(pDUMP,"�b"); break;
					case 0xB9: fprintf(pDUMP,"��"); break;
					case 0xBA: fprintf(pDUMP,"��"); break;
					case 0xBB: fprintf(pDUMP,"��"); break;
					case 0xBC: fprintf(pDUMP,"�@"); break;
					case 0xBD: fprintf(pDUMP,"�B"); break;
					case 0xBE: fprintf(pDUMP,"�D"); break;
					case 0xBF: fprintf(pDUMP,"�F"); break;
					case 0xC0: fprintf(pDUMP,"�H"); break;
					case 0xC1: fprintf(pDUMP,"�K"); break;
					case 0xC2: fprintf(pDUMP,"�M"); break;
					case 0xC3: fprintf(pDUMP,"�O"); break;
					case 0xC4: fprintf(pDUMP,"�Q"); break;
					case 0xC5: fprintf(pDUMP,"�S"); break;
					case 0xC6: fprintf(pDUMP,"�U"); break;
					case 0xC7: fprintf(pDUMP,"�W"); break;
					case 0xC8: fprintf(pDUMP,"�Y"); break;
					case 0xC9: fprintf(pDUMP,"�["); break;
					case 0xCA: fprintf(pDUMP,"�]"); break;
					case 0xCB: fprintf(pDUMP,"�_"); break;
					case 0xCC: fprintf(pDUMP,"�a"); break;
					case 0xCD: fprintf(pDUMP,"�d"); break;
					case 0xCE: fprintf(pDUMP,"�f"); break;
					case 0xCF: fprintf(pDUMP,"�h"); break;
					case 0xD0: fprintf(pDUMP,"�o"); break;
					case 0xD1: fprintf(pDUMP,"�r"); break;
					case 0xD2: fprintf(pDUMP,"�u"); break;
					case 0xD3: fprintf(pDUMP,"�x"); break;
					case 0xD4: fprintf(pDUMP,"�{"); break;
					case 0xD5: fprintf(pDUMP,"�p"); break;
					case 0xD6: fprintf(pDUMP,"�s"); break;
					case 0xD7: fprintf(pDUMP,"�v"); break;
					case 0xD8: fprintf(pDUMP,"�y"); break;
					case 0xD9: fprintf(pDUMP,"�|"); break;
					case 0xDA: fprintf(pDUMP,"��"); break;
					case 0xDB: fprintf(pDUMP,"��"); break;
					case 0xDC: fprintf(pDUMP,"��"); break;
					case 0xDD: fprintf(pDUMP,"��"); break;
					case 0xDE: fprintf(pDUMP,"��"); break;
					case 0xDF: fprintf(pDUMP,"��"); break;
					case 0xE0: fprintf(pDUMP,"��"); break;
					case 0xE1: fprintf(pDUMP,"��"); break;
					case 0xE2: fprintf(pDUMP,"��"); break;
					case 0xE3: fprintf(pDUMP,"��"); break;
					case 0xE4: fprintf(pDUMP,"��"); break;
					case 0xE5: fprintf(pDUMP,"��"); break;
					case 0xE6: fprintf(pDUMP,"��"); break;
					case 0xE7: fprintf(pDUMP,"��"); break;
					case 0xE8: fprintf(pDUMP,"��"); break;
					case 0xE9: fprintf(pDUMP,"��"); break;
					case 0xEA: fprintf(pDUMP,"��"); break;
					case 0xEB: fprintf(pDUMP,"��"); break;
					case 0xEC: fprintf(pDUMP,"��"); break;
					case 0xED: fprintf(pDUMP,"��"); break;
					case 0xEE: fprintf(pDUMP,"��"); break;
					case 0xEF: fprintf(pDUMP,"��"); break;
					case 0xF0: fprintf(pDUMP,"��"); break;
					case 0xF1: fprintf(pDUMP,"��"); break;
					case 0xF2: fprintf(pDUMP,"��"); break;
					case 0xF3: fprintf(pDUMP,"��"); break;
					case 0xF4: fprintf(pDUMP,"��"); break;
					case 0xF5: fprintf(pDUMP,"�X"); break;
					case 0xF6: fprintf(pDUMP,"��"); break;
					case 0xF7: fprintf(pDUMP,"��"); break;
					case 0xF8: fprintf(pDUMP,"��"); break;
					case 0xF9: fprintf(pDUMP,"��"); break;
					case 0xFA: fprintf(pDUMP,"�p"); break;
					case 0xFB: fprintf(pDUMP,"��"); break;
					case 0xFC: fprintf(pDUMP,"��"); break;
					case 0xFD: fprintf(pDUMP,"�N"); break;
					case 0xFE: fprintf(pDUMP,"��"); break;
					case 0xFF: fprintf(pDUMP,"��"); break;
				}
			}
		}
		//shift 2
		else if(cur >= 0x68 && cur <= 0x6B)
		{
			if(newString){fprintf(pDUMP,"\n<%0.6X>\n",addr); newString = false;}
			
			unsigned char shiftLength = (cur-0x68);
			for(int i=0; i <= shiftLength; i++)
			{
				advance(cur,pROM,addr);
				
				col++; // advance current col
				if (col == 16)
				{
					col = 0;
					fprintf(pDUMP,"<WRAP>\n");
				}
				
				switch (cur)
				{
					case 0x0: fprintf(pDUMP,"�O"); break;
					case 0x1: fprintf(pDUMP,"�P"); break;
					case 0x2: fprintf(pDUMP,"�Q"); break;
					case 0x3: fprintf(pDUMP,"�R"); break;
					case 0x4: fprintf(pDUMP,"�S"); break;
					case 0x5: fprintf(pDUMP,"�T"); break;
					case 0x6: fprintf(pDUMP,"�U"); break;
					case 0x7: fprintf(pDUMP,"�V"); break;
					case 0x8: fprintf(pDUMP,"�W"); break;
					case 0x9: fprintf(pDUMP,"�X"); break;
					case 0x0A: fprintf(pDUMP,"�A"); break;
					case 0x0B: fprintf(pDUMP,"�C"); break;
					case 0x0C: fprintf(pDUMP,"�E"); break;
					case 0x0D: fprintf(pDUMP,"�G"); break;
					case 0x0E: fprintf(pDUMP,"�I"); break;
					case 0x0F: fprintf(pDUMP,"�J"); break;
					case 0x10: fprintf(pDUMP,"�L"); break;
					case 0x11: fprintf(pDUMP,"�N"); break;
					case 0x12: fprintf(pDUMP,"�P"); break;
					case 0x13: fprintf(pDUMP,"�R"); break;
					case 0x14: fprintf(pDUMP,"�T"); break;
					case 0x15: fprintf(pDUMP,"�V"); break;
					case 0x16: fprintf(pDUMP,"�X"); break;
					case 0x17: fprintf(pDUMP,"�Z"); break;
					case 0x18: fprintf(pDUMP,"�\\"); break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x19: fprintf(pDUMP,"�^"); break;
					case 0x1A: fprintf(pDUMP,"�`"); break;
					case 0x1B: fprintf(pDUMP,"�c"); break;
					case 0x1C: fprintf(pDUMP,"�e"); break;
					case 0x1D: fprintf(pDUMP,"�g"); break;
					case 0x1E: fprintf(pDUMP,"�i"); break;
					case 0x1F: fprintf(pDUMP,"�j"); break;
					case 0x20: fprintf(pDUMP,"�k"); break;
					case 0x21: fprintf(pDUMP,"�l"); break;
					case 0x22: fprintf(pDUMP,"�m"); break;
					case 0x23: fprintf(pDUMP,"�n"); break;
					case 0x24: fprintf(pDUMP,"�q"); break;
					case 0x25: fprintf(pDUMP,"�t"); break;
					case 0x26: fprintf(pDUMP,"�w"); break;
					case 0x27: fprintf(pDUMP,"�z"); break;
					case 0x28: fprintf(pDUMP,"�}"); break;
					case 0x29: fprintf(pDUMP,"�~"); break;
					case 0x2A: fprintf(pDUMP,"��"); break;
					case 0x2B: fprintf(pDUMP,"��"); break;
					case 0x2C: fprintf(pDUMP,"��"); break;
					case 0x2D: fprintf(pDUMP,"��"); break;
					case 0x2E: fprintf(pDUMP,"��"); break;
					case 0x2F: fprintf(pDUMP,"��"); break;
					case 0x30: fprintf(pDUMP,"��"); break;
					case 0x31: fprintf(pDUMP,"��"); break;
					case 0x32: fprintf(pDUMP,"��"); break;
					case 0x33: fprintf(pDUMP,"��"); break;
					case 0x34: fprintf(pDUMP,"��"); break;
					case 0x35: fprintf(pDUMP,"��"); break;
					case 0x36: fprintf(pDUMP,"��"); break;
					case 0x37: fprintf(pDUMP,"��"); break;
					case 0x38: fprintf(pDUMP,"�b"); break;
					case 0x39: fprintf(pDUMP,"��"); break;
					case 0x3A: fprintf(pDUMP,"��"); break;
					case 0x3B: fprintf(pDUMP,"��"); break;
					case 0x3C: fprintf(pDUMP,"�@"); break;
					case 0x3D: fprintf(pDUMP,"�B"); break;
					case 0x3E: fprintf(pDUMP,"�D"); break;
					case 0x3F: fprintf(pDUMP,"�F"); break;
					case 0x40: fprintf(pDUMP,"�H"); break;
					case 0x41: fprintf(pDUMP,"�K"); break;
					case 0x42: fprintf(pDUMP,"�M"); break;
					case 0x43: fprintf(pDUMP,"�O"); break;
					case 0x44: fprintf(pDUMP,"�Q"); break;
					case 0x45: fprintf(pDUMP,"�S"); break;
					case 0x46: fprintf(pDUMP,"�U"); break;
					case 0x47: fprintf(pDUMP,"�W"); break;
					case 0x48: fprintf(pDUMP,"�Y"); break;
					case 0x49: fprintf(pDUMP,"�["); break;
					case 0x4A: fprintf(pDUMP,"�]"); break;
					case 0x4B: fprintf(pDUMP,"�_"); break;
					case 0x4C: fprintf(pDUMP,"�a"); break;
					case 0x4D: fprintf(pDUMP,"�d"); break;
					case 0x4E: fprintf(pDUMP,"�f"); break;
					case 0x4F: fprintf(pDUMP,"�h"); break;
					case 0x50: fprintf(pDUMP,"�o"); break;
					case 0x51: fprintf(pDUMP,"�r"); break;
					case 0x52: fprintf(pDUMP,"�u"); break;
					case 0x53: fprintf(pDUMP,"�x"); break;
					case 0x54: fprintf(pDUMP,"�{"); break;
					case 0x55: fprintf(pDUMP,"�p"); break;
					case 0x56: fprintf(pDUMP,"�s"); break;
					case 0x57: fprintf(pDUMP,"�v"); break;
					case 0x58: fprintf(pDUMP,"�y"); break;
					case 0x59: fprintf(pDUMP,"�|"); break;
					case 0x5A: fprintf(pDUMP,"��"); break;
					case 0x5B: fprintf(pDUMP,"��"); break;
					case 0x5C: fprintf(pDUMP,"��"); break;
					case 0x5D: fprintf(pDUMP,"��"); break;
					case 0x5E: fprintf(pDUMP,"��"); break;
					case 0x5F: fprintf(pDUMP,"��"); break;
					case 0x60: fprintf(pDUMP,"��"); break;
					case 0x61: fprintf(pDUMP,"��"); break;
					case 0x62: fprintf(pDUMP,"��"); break;
					case 0x63: fprintf(pDUMP,"��"); break;
					case 0x64: fprintf(pDUMP,"��"); break;
					case 0x65: fprintf(pDUMP,"��"); break;
					case 0x66: fprintf(pDUMP,"��"); break;
					case 0x67: fprintf(pDUMP,"��"); break;
					case 0x68: fprintf(pDUMP,"��"); break;
					case 0x69: fprintf(pDUMP,"��"); break;
					case 0x6A: fprintf(pDUMP,"��"); break;
					case 0x6B: fprintf(pDUMP,"��"); break;
					case 0x6C: fprintf(pDUMP,"��"); break;
					case 0x6D: fprintf(pDUMP,"��"); break;
					case 0x6E: fprintf(pDUMP,"��"); break;
					case 0x6F: fprintf(pDUMP,"��"); break;
					case 0x70: fprintf(pDUMP,"��"); break;
					case 0x71: fprintf(pDUMP,"��"); break;
					case 0x72: fprintf(pDUMP,"��"); break;
					case 0x73: fprintf(pDUMP,"��"); break;
					case 0x74: fprintf(pDUMP,"��"); break;
					case 0x75: fprintf(pDUMP,"�X"); break;
					case 0x76: fprintf(pDUMP,"��"); break;
					case 0x77: fprintf(pDUMP,"��"); break;
					case 0x78: fprintf(pDUMP,"��"); break;
					case 0x79: fprintf(pDUMP,"��"); break;
					case 0x7A: fprintf(pDUMP,"�p"); break;
					case 0x7B: fprintf(pDUMP,"��"); break;
					case 0x7C: fprintf(pDUMP,"��"); break;
					case 0x7D: fprintf(pDUMP,"�N"); break;
					case 0x7E: fprintf(pDUMP,"��"); break;
					case 0x7F: fprintf(pDUMP,"��"); break;
					case 0x80: fprintf(pDUMP,"��"); break;
					case 0x81: fprintf(pDUMP,"�l"); break;
					case 0x82: fprintf(pDUMP,"��"); break;
					case 0x83: fprintf(pDUMP,"�X"); break;
					case 0x84: fprintf(pDUMP,"��"); break;
					case 0x85: fprintf(pDUMP,"��"); break;
					case 0x86: fprintf(pDUMP,"�v"); break;
					case 0x87: fprintf(pDUMP,"��"); break;
					case 0x88: fprintf(pDUMP,"��"); break;
					case 0x89: fprintf(pDUMP,"��"); break;
					case 0x8A: fprintf(pDUMP,"�`"); break;
					case 0x8B: fprintf(pDUMP,"��"); break;
					case 0x8C: fprintf(pDUMP,"��"); break;
					case 0x8D: fprintf(pDUMP,"��"); break;
					case 0x8E: fprintf(pDUMP,"�V"); break;
					case 0x8F: fprintf(pDUMP,"��"); break;
					case 0x90: fprintf(pDUMP,"��"); break;
					case 0x91: fprintf(pDUMP,"��"); break;
					case 0x92: fprintf(pDUMP,"�j"); break;
					case 0x93: fprintf(pDUMP,"��"); break;
					case 0x94: fprintf(pDUMP,"��"); break;
					case 0x95: fprintf(pDUMP,"�W"); break;
					case 0x96: fprintf(pDUMP,"�^"); break;
					case 0x97: fprintf(pDUMP,"�k"); break;
					case 0x98: fprintf(pDUMP,"��"); break;
					case 0x99: fprintf(pDUMP,"��"); break;
					case 0x9A: fprintf(pDUMP,"�n"); break;
					case 0x9B: fprintf(pDUMP,"��"); break;
					case 0x9C: fprintf(pDUMP,"��"); break;
					case 0x9D: fprintf(pDUMP,"��"); break;
					case 0x9E: fprintf(pDUMP,"��"); break;
					case 0x9F: fprintf(pDUMP,"��"); break;
					case 0xA0: fprintf(pDUMP,"�j"); break;
					case 0xA1: fprintf(pDUMP,"��"); break;
					case 0xA2: fprintf(pDUMP,"�X"); break;
					case 0xA3: fprintf(pDUMP,"��"); break;
					case 0xA4: fprintf(pDUMP,"��"); break;
					case 0xA5: fprintf(pDUMP,"��"); break;
					case 0xA6: fprintf(pDUMP,"��"); break;
					case 0xA7: fprintf(pDUMP,"��"); break;
					case 0xA8: fprintf(pDUMP,"��"); break;
					case 0xA9: fprintf(pDUMP,"�q"); break;
					case 0xAA: fprintf(pDUMP,"��"); break;
					case 0xAB: fprintf(pDUMP,"�s"); break;
					case 0xAC: fprintf(pDUMP,"��"); break;
					case 0xAD: fprintf(pDUMP,"��"); break;
					case 0xAE: fprintf(pDUMP,"�c"); break;
					case 0xAF: fprintf(pDUMP,"�v"); break;
					case 0xB0: fprintf(pDUMP,"��"); break;
					case 0xB1: fprintf(pDUMP,"�J"); break;
					case 0xB2: fprintf(pDUMP,"��"); break;
					case 0xB3: fprintf(pDUMP,"��"); break;
					case 0xB4: fprintf(pDUMP,"�N"); break;
					case 0xB5: fprintf(pDUMP,"��"); break;
					case 0xB6: fprintf(pDUMP,"��"); break;
					case 0xB7: fprintf(pDUMP,"��"); break;
					case 0xB8: fprintf(pDUMP,"��"); break;
					case 0xB9: fprintf(pDUMP,"��"); break;
					case 0xBA: fprintf(pDUMP,"��"); break;
					case 0xBB: fprintf(pDUMP,"��"); break;
					case 0xBC: fprintf(pDUMP,"�R"); break;
					case 0xBD: fprintf(pDUMP,"�m"); break;
					case 0xBE: fprintf(pDUMP,"�G"); break;
					case 0xBF: fprintf(pDUMP,"��"); break;
					case 0xC0: fprintf(pDUMP,"��"); break;
					case 0xC1: fprintf(pDUMP,"��"); break;
					case 0xC2: fprintf(pDUMP,"��"); break;
					case 0xC3: fprintf(pDUMP,"��"); break;
					case 0xC4: fprintf(pDUMP,"��"); break;
					case 0xC5: fprintf(pDUMP,"��"); break;
					case 0xC6: fprintf(pDUMP,"�v"); break;
					case 0xC7: fprintf(pDUMP,"��"); break;
					case 0xC8: fprintf(pDUMP,"��"); break;
					case 0xC9: fprintf(pDUMP,"��"); break;
					case 0xCA: fprintf(pDUMP,"��"); break;
					case 0xCB: fprintf(pDUMP,"��"); break;
					case 0xCC: fprintf(pDUMP,"�O"); break;
					case 0xCD: fprintf(pDUMP,"��"); break;
					case 0xCE: fprintf(pDUMP,"��"); break;
					case 0xCF: fprintf(pDUMP,"�g"); break;
					case 0xD0: fprintf(pDUMP,"�{"); break;
					case 0xD1: fprintf(pDUMP,"��"); break;
					case 0xD2: fprintf(pDUMP,"��"); break;
					case 0xD3: fprintf(pDUMP,"�B"); break;
					case 0xD4: fprintf(pDUMP,"��"); break;
					case 0xD5: fprintf(pDUMP,"��"); break;
					case 0xD6: fprintf(pDUMP,"�C"); break;
					case 0xD7: fprintf(pDUMP,"��"); break;
					case 0xD8: fprintf(pDUMP,"�b"); break;
					case 0xD9: fprintf(pDUMP,"�E"); break;
					case 0xDA: fprintf(pDUMP,"�S"); break;
					case 0xDB: fprintf(pDUMP,"��"); break;
					case 0xDC: fprintf(pDUMP,"�D"); break;
					case 0xDD: fprintf(pDUMP,"��"); break;
					case 0xDE: fprintf(pDUMP,"��"); break;
					case 0xDF: fprintf(pDUMP,"��"); break;
					case 0xE0: fprintf(pDUMP,"��"); break;
					case 0xE1: fprintf(pDUMP,"��"); break;
					case 0xE2: fprintf(pDUMP,"��"); break;
					case 0xE3: fprintf(pDUMP,"�N"); break;
					case 0xE4: fprintf(pDUMP,"�a"); break;
					case 0xE5: fprintf(pDUMP,"��"); break;
					case 0xE6: fprintf(pDUMP,"��"); break;
					case 0xE7: fprintf(pDUMP,"��"); break;
					case 0xE8: fprintf(pDUMP,"��"); break;
					case 0xE9: fprintf(pDUMP,"��"); break;
					case 0xEA: fprintf(pDUMP,"�n"); break;
					case 0xEB: fprintf(pDUMP,"��"); break;
					case 0xEC: fprintf(pDUMP,"��"); break;
					case 0xED: fprintf(pDUMP,"�_"); break;
					case 0xEE: fprintf(pDUMP,"��"); break;
					case 0xEF: fprintf(pDUMP,"��"); break;
					case 0xF0: fprintf(pDUMP,"�_"); break;
					case 0xF1: fprintf(pDUMP,"��"); break;
					case 0xF2: fprintf(pDUMP,"�F"); break;
					case 0xF3: fprintf(pDUMP,"��"); break;
					case 0xF4: fprintf(pDUMP,"��"); break;
					case 0xF5: fprintf(pDUMP,"��"); break;
					case 0xF6: fprintf(pDUMP,"�R"); break;
					case 0xF7: fprintf(pDUMP,"�w"); break;
					case 0xF8: fprintf(pDUMP,"��"); break;
					case 0xF9: fprintf(pDUMP,"��"); break;
					case 0xFA: fprintf(pDUMP,"��"); break;
					case 0xFB: fprintf(pDUMP,"MP"); break;
					case 0xFC: fprintf(pDUMP,"��"); break;
					case 0xFD: fprintf(pDUMP,"��"); break;
					case 0xFE: fprintf(pDUMP,"��"); break;
					case 0xFF: fprintf(pDUMP,"��"); break;
				}
			}
		}
		//shift 3
		else if(cur >= 0x6C && cur <= 0x6F)
		{
			if(newString){fprintf(pDUMP,"\n<%0.6X>\n",addr); newString = false;}
			
			unsigned char shiftLength = (cur-0x6C);
			for(int i=0; i <= shiftLength; i++)
			{
				advance(cur,pROM,addr);
				
				col++; // advance current col
				if (col == 16)
				{
					col = 0;
					fprintf(pDUMP,"<WRAP>\n");
				}
				
				switch (cur)
				{
					case 0x0: fprintf(pDUMP,"��"); break;
					case 0x1: fprintf(pDUMP,"�l"); break;
					case 0x2: fprintf(pDUMP,"��"); break;
					case 0x3: fprintf(pDUMP,"�X"); break;
					case 0x4: fprintf(pDUMP,"��"); break;
					case 0x5: fprintf(pDUMP,"��"); break;
					case 0x6: fprintf(pDUMP,"�v"); break;
					case 0x7: fprintf(pDUMP,"��"); break;
					case 0x8: fprintf(pDUMP,"��"); break;
					case 0x9: fprintf(pDUMP,"��"); break;
					case 0x0A: fprintf(pDUMP,"�`"); break;
					case 0x0B: fprintf(pDUMP,"��"); break;
					case 0x0C: fprintf(pDUMP,"��"); break;
					case 0x0D: fprintf(pDUMP,"��"); break;
					case 0x0E: fprintf(pDUMP,"�V"); break;
					case 0x0F: fprintf(pDUMP,"��"); break;
					case 0x10: fprintf(pDUMP,"��"); break;
					case 0x11: fprintf(pDUMP,"��"); break;
					case 0x12: fprintf(pDUMP,"�j"); break;
					case 0x13: fprintf(pDUMP,"��"); break;
					case 0x14: fprintf(pDUMP,"��"); break;
					case 0x15: fprintf(pDUMP,"�W"); break;
					case 0x16: fprintf(pDUMP,"�^"); break;
					case 0x17: fprintf(pDUMP,"�k"); break;
					case 0x18: fprintf(pDUMP,"��"); break;
					case 0x19: fprintf(pDUMP,"��"); break;
					case 0x1A: fprintf(pDUMP,"�n"); break;
					case 0x1B: fprintf(pDUMP,"��"); break;
					case 0x1C: fprintf(pDUMP,"��"); break;
					case 0x1D: fprintf(pDUMP,"��"); break;
					case 0x1E: fprintf(pDUMP,"��"); break;
					case 0x1F: fprintf(pDUMP,"��"); break;
					case 0x20: fprintf(pDUMP,"�j"); break;
					case 0x21: fprintf(pDUMP,"��"); break;
					case 0x22: fprintf(pDUMP,"�X"); break;
					case 0x23: fprintf(pDUMP,"��"); break;
					case 0x24: fprintf(pDUMP,"��"); break;
					case 0x25: fprintf(pDUMP,"��"); break;
					case 0x26: fprintf(pDUMP,"��"); break;
					case 0x27: fprintf(pDUMP,"��"); break;
					case 0x28: fprintf(pDUMP,"��"); break;
					case 0x29: fprintf(pDUMP,"�q"); break;
					case 0x2A: fprintf(pDUMP,"��"); break;
					case 0x2B: fprintf(pDUMP,"�s"); break;
					case 0x2C: fprintf(pDUMP,"��"); break;
					case 0x2D: fprintf(pDUMP,"��"); break;
					case 0x2E: fprintf(pDUMP,"�c"); break;
					case 0x2F: fprintf(pDUMP,"�v"); break;
					case 0x30: fprintf(pDUMP,"��"); break;
					case 0x31: fprintf(pDUMP,"�J"); break;
					case 0x32: fprintf(pDUMP,"��"); break;
					case 0x33: fprintf(pDUMP,"��"); break;
					case 0x34: fprintf(pDUMP,"�N"); break;
					case 0x35: fprintf(pDUMP,"��"); break;
					case 0x36: fprintf(pDUMP,"��"); break;
					case 0x37: fprintf(pDUMP,"��"); break;
					case 0x38: fprintf(pDUMP,"��"); break;
					case 0x39: fprintf(pDUMP,"��"); break;
					case 0x3A: fprintf(pDUMP,"��"); break;
					case 0x3B: fprintf(pDUMP,"��"); break;
					case 0x3C: fprintf(pDUMP,"�R"); break;
					case 0x3D: fprintf(pDUMP,"�m"); break;
					case 0x3E: fprintf(pDUMP,"�G"); break;
					case 0x3F: fprintf(pDUMP,"��"); break;
					case 0x40: fprintf(pDUMP,"��"); break;
					case 0x41: fprintf(pDUMP,"��"); break;
					case 0x42: fprintf(pDUMP,"��"); break;
					case 0x43: fprintf(pDUMP,"��"); break;
					case 0x44: fprintf(pDUMP,"��"); break;
					case 0x45: fprintf(pDUMP,"��"); break;
					case 0x46: fprintf(pDUMP,"�v"); break;
					case 0x47: fprintf(pDUMP,"��"); break;
					case 0x48: fprintf(pDUMP,"��"); break;
					case 0x49: fprintf(pDUMP,"��"); break;
					case 0x4A: fprintf(pDUMP,"��"); break;
					case 0x4B: fprintf(pDUMP,"��"); break;
					case 0x4C: fprintf(pDUMP,"�O"); break;
					case 0x4D: fprintf(pDUMP,"��"); break;
					case 0x4E: fprintf(pDUMP,"��"); break;
					case 0x4F: fprintf(pDUMP,"�g"); break;
					case 0x50: fprintf(pDUMP,"�{"); break;
					case 0x51: fprintf(pDUMP,"��"); break;
					case 0x52: fprintf(pDUMP,"��"); break;
					case 0x53: fprintf(pDUMP,"�B"); break;
					case 0x54: fprintf(pDUMP,"��"); break;
					case 0x55: fprintf(pDUMP,"��"); break;
					case 0x56: fprintf(pDUMP,"�C"); break;
					case 0x57: fprintf(pDUMP,"��"); break;
					case 0x58: fprintf(pDUMP,"�b"); break;
					case 0x59: fprintf(pDUMP,"�E"); break;
					case 0x5A: fprintf(pDUMP,"�S"); break;
					case 0x5B: fprintf(pDUMP,"��"); break;
					case 0x5C: fprintf(pDUMP,"�D"); break;
					case 0x5D: fprintf(pDUMP,"��"); break;
					case 0x5E: fprintf(pDUMP,"��"); break;
					case 0x5F: fprintf(pDUMP,"��"); break;
					case 0x60: fprintf(pDUMP,"��"); break;
					case 0x61: fprintf(pDUMP,"��"); break;
					case 0x62: fprintf(pDUMP,"��"); break;
					case 0x63: fprintf(pDUMP,"�N"); break;
					case 0x64: fprintf(pDUMP,"�a"); break;
					case 0x65: fprintf(pDUMP,"��"); break;
					case 0x66: fprintf(pDUMP,"��"); break;
					case 0x67: fprintf(pDUMP,"��"); break;
					case 0x68: fprintf(pDUMP,"��"); break;
					case 0x69: fprintf(pDUMP,"��"); break;
					case 0x6A: fprintf(pDUMP,"�n"); break;
					case 0x6B: fprintf(pDUMP,"��"); break;
					case 0x6C: fprintf(pDUMP,"��"); break;
					case 0x6D: fprintf(pDUMP,"�_"); break;
					case 0x6E: fprintf(pDUMP,"��"); break;
					case 0x6F: fprintf(pDUMP,"��"); break;
					case 0x70: fprintf(pDUMP,"�_"); break;
					case 0x71: fprintf(pDUMP,"��"); break;
					case 0x72: fprintf(pDUMP,"�F"); break;
					case 0x73: fprintf(pDUMP,"��"); break;
					case 0x74: fprintf(pDUMP,"��"); break;
					case 0x75: fprintf(pDUMP,"��"); break;
					case 0x76: fprintf(pDUMP,"�R"); break;
					case 0x77: fprintf(pDUMP,"�w"); break;
					case 0x78: fprintf(pDUMP,"��"); break;
					case 0x79: fprintf(pDUMP,"��"); break;
					case 0x7A: fprintf(pDUMP,"��"); break;
					case 0x7B: fprintf(pDUMP,"MP"); break;
					case 0x7C: fprintf(pDUMP,"��"); break;
					case 0x7D: fprintf(pDUMP,"��"); break;
					case 0x7E: fprintf(pDUMP,"��"); break;
					case 0x7F: fprintf(pDUMP,"��"); break;
					case 0x80: fprintf(pDUMP,"��"); break;
					case 0x81: fprintf(pDUMP,"��"); break;
					case 0x82: fprintf(pDUMP,"��"); break;
					case 0x83: fprintf(pDUMP,"��"); break;
					case 0x84: fprintf(pDUMP,"�\\"); break;//more wierdness
					case 0x85: fprintf(pDUMP,"��"); break;
					case 0x86: fprintf(pDUMP,"�I"); break;
					case 0x87: fprintf(pDUMP,"��"); break;
					case 0x88: fprintf(pDUMP,"��"); break;
					case 0x89: fprintf(pDUMP,"��"); break;
					case 0x8A: fprintf(pDUMP,"�E"); break;
					case 0x8B: fprintf(pDUMP,"�i"); break;
					case 0x8C: fprintf(pDUMP,"��"); break;
					case 0x8D: fprintf(pDUMP,"��"); break;
					case 0x8E: fprintf(pDUMP,"��"); break;
					case 0x8F: fprintf(pDUMP,"��"); break;
					case 0x90: fprintf(pDUMP,"�@"); break;
					case 0x91: fprintf(pDUMP,"��"); break;
					case 0x92: fprintf(pDUMP,"�l"); break;
					case 0x93: fprintf(pDUMP,"��"); break;
					case 0x94: fprintf(pDUMP,"��"); break;
					case 0x95: fprintf(pDUMP,"��"); break;
					case 0x96: fprintf(pDUMP,"��"); break;
					case 0x97: fprintf(pDUMP,"�e"); break;
					case 0x98: fprintf(pDUMP,"�U"); break;
					case 0x99: fprintf(pDUMP,"�n"); break;
					case 0x9A: fprintf(pDUMP,"��"); break;
					case 0x9B: fprintf(pDUMP,"��"); break;
					case 0x9C: fprintf(pDUMP,"��"); break;
					case 0x9D: fprintf(pDUMP,"�I"); break;
					case 0x9E: fprintf(pDUMP,"�h"); break;
					case 0x9F: fprintf(pDUMP,"��"); break;
					case 0xA0: fprintf(pDUMP,"��"); break;
					case 0xA1: fprintf(pDUMP,"��"); break;
					case 0xA2: fprintf(pDUMP,"��"); break;
					case 0xA3: fprintf(pDUMP,"��"); break;
					case 0xA4: fprintf(pDUMP,"��"); break;
					case 0xA5: fprintf(pDUMP,"��"); break;
					case 0xA6: fprintf(pDUMP,"��"); break;
					case 0xA7: fprintf(pDUMP,"��"); break;
					case 0xA8: fprintf(pDUMP,"�Z"); break;
					case 0xA9: fprintf(pDUMP,"��"); break;
					case 0xAA: fprintf(pDUMP,"��"); break;
					case 0xAB: fprintf(pDUMP,"��"); break;
					case 0xAC: fprintf(pDUMP,"��"); break;
					case 0xAD: fprintf(pDUMP,"��"); break;
					case 0xAE: fprintf(pDUMP,"��"); break;
					case 0xAF: fprintf(pDUMP,"�a"); break;
					case 0xB0: fprintf(pDUMP,"�m"); break;
					case 0xB1: fprintf(pDUMP,"��"); break;
					case 0xB2: fprintf(pDUMP,"�X"); break;
					case 0xB3: fprintf(pDUMP,"�D"); break;
					case 0xB4: fprintf(pDUMP,"��"); break;
					case 0xB5: fprintf(pDUMP,"�_"); break;
					case 0xB6: fprintf(pDUMP,"��"); break;
					case 0xB7: fprintf(pDUMP,"�O"); break;
					case 0xB8: fprintf(pDUMP,"��"); break;
					case 0xB9: fprintf(pDUMP,"��"); break;
					case 0xBA: fprintf(pDUMP,"��"); break;
					case 0xBB: fprintf(pDUMP,"��"); break;
					case 0xBC: fprintf(pDUMP,"��"); break;
					case 0xBD: fprintf(pDUMP,"�E"); break;
					case 0xBE: fprintf(pDUMP,"�b"); break;
					case 0xBF: fprintf(pDUMP,"��"); break;
					case 0xC0: fprintf(pDUMP,"�@"); break;
					case 0xC1: fprintf(pDUMP,"��"); break;
					case 0xC2: fprintf(pDUMP,"��"); break;
					case 0xC3: fprintf(pDUMP,"��"); break;
					case 0xC4: fprintf(pDUMP,"��"); break;
					case 0xC5: fprintf(pDUMP,"�E"); break;
					case 0xC6: fprintf(pDUMP,"�C"); break;
					case 0xC7: fprintf(pDUMP,"��"); break;
					case 0xC8: fprintf(pDUMP,"�b"); break;
					case 0xC9: fprintf(pDUMP,"�S"); break;
					case 0xCA: fprintf(pDUMP,"��"); break;
					case 0xCB: fprintf(pDUMP,"��"); break;
					case 0xCC: fprintf(pDUMP,"��"); break;
					case 0xCD: fprintf(pDUMP,"��"); break;
					case 0xCE: fprintf(pDUMP,"�U"); break;
					case 0xCF: fprintf(pDUMP,"�N"); break;
					case 0xD0: fprintf(pDUMP,"��"); break;
					case 0xD1: fprintf(pDUMP,"��"); break;
					case 0xD2: fprintf(pDUMP,"��"); break;
					case 0xD3: fprintf(pDUMP,"��"); break;
					case 0xD4: fprintf(pDUMP,"�V"); break;
					case 0xD5: fprintf(pDUMP,"��"); break;
					case 0xD6: fprintf(pDUMP,"��"); break;
					case 0xD7: fprintf(pDUMP,"�g"); break;
					case 0xD8: fprintf(pDUMP,"��"); break;
					case 0xD9: fprintf(pDUMP,"��"); break;
					case 0xDA: fprintf(pDUMP,"��"); break;
					case 0xDB: fprintf(pDUMP,"�v"); break;
					case 0xDC: fprintf(pDUMP,"��"); break;
					case 0xDD: fprintf(pDUMP,"��"); break;
					case 0xDE: fprintf(pDUMP,"��"); break;
					case 0xDF: fprintf(pDUMP,"�H"); break;
					case 0xE0: fprintf(pDUMP,"��"); break;
					case 0xE1: fprintf(pDUMP,"��"); break;
					case 0xE2: fprintf(pDUMP,"�o"); break;
					case 0xE3: fprintf(pDUMP,"��"); break;
					case 0xE4: fprintf(pDUMP,"�q"); break;
					case 0xE5: fprintf(pDUMP,"�C"); break;
					case 0xE6: fprintf(pDUMP,"��"); break;
					case 0xE7: fprintf(pDUMP,"�e"); break;
					case 0xE8: fprintf(pDUMP,"�j"); break;
					case 0xE9: fprintf(pDUMP,"�t"); break;
					case 0xEA: fprintf(pDUMP,"��"); break;
					case 0xEB: fprintf(pDUMP,"��"); break;
					case 0xEC: fprintf(pDUMP,"��"); break;
					case 0xED: fprintf(pDUMP,"��"); break;
					case 0xEE: fprintf(pDUMP,"��"); break;
					case 0xEF: fprintf(pDUMP,"��"); break;
					case 0xF0: fprintf(pDUMP,"��"); break;
					case 0xF1: fprintf(pDUMP,"��"); break;
					case 0xF2: fprintf(pDUMP,"��"); break;
					case 0xF3: fprintf(pDUMP,"��"); break;
					case 0xF4: fprintf(pDUMP,"��"); break;
					case 0xF5: fprintf(pDUMP,"��"); break;
					case 0xF6: fprintf(pDUMP,"��"); break;
					case 0xF7: fprintf(pDUMP,"�|"); break;
					case 0xF8: fprintf(pDUMP,"�S"); break;
					case 0xF9: fprintf(pDUMP,"�\\"); break;//more wierdness
					case 0xFA: fprintf(pDUMP,"��"); break;
					case 0xFB: fprintf(pDUMP,"�d"); break;
					case 0xFC: fprintf(pDUMP,"�C"); break;
					case 0xFD: fprintf(pDUMP,"��"); break;
					case 0xFE: fprintf(pDUMP,"��"); break;
					case 0xFF: fprintf(pDUMP,"��"); break;
				}
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
