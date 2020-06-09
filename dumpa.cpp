#include<stdio.h>

//TO-DO:
//goal: match appearance of in game textbox (bar debug output)
//check rest of stuff in playNotes
//check for EOS misfires
//store/shop dialogue
//fix string coding stuff
//in dump, don't base box spacing on <pause>. maybe count rows->3 or something?
//option to A) show exactly like game or B) show special codes too
//		maybe can pick between the types. ie textbox start/end, line end, and internal effects

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

int main()
{
	unsigned char cur = 0x00; //contents of current byte of the ROM
	char* moji = "–³"; //moji to print
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
				fprintf(pDUMP,"<ƒ‰ƒ“ƒfƒB>");
				col += 4;
			} 
			else if(cur == 0x01)
			{
				fprintf(pDUMP,"<ƒvƒŠƒ€>");
				col += 3;
			}
			else if(cur == 0x02)
			{
				fprintf(pDUMP,"<ƒ|ƒ|ƒC>");
				col += 3;
			}
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
			    case 0x80: moji = "A";  break;
				case 0x81: moji = "B";  break;
				case 0x82: moji = "I";  break;
				case 0x83: moji = "H";  break;
				case 0x84: moji = "[";  break;
				case 0x85: moji = "d";  break;
				case 0x86: moji = "u";  break;
				case 0x87: moji = "v";  break;
				case 0x88: moji = "w";  break;
				case 0x89: moji = "x";  break;
				case 0x8A: moji = "‚ ";  break;
				case 0x8B: moji = "‚¢";  break;
				case 0x8C: moji = "‚¤";  break;
				case 0x8D: moji = "‚¦";  break;
				case 0x8E: moji = "‚¨";  break;
				case 0x8F: moji = "‚©";  break;
				case 0x90: moji = "‚«";  break;
				case 0x91: moji = "‚­";  break;
				case 0x92: moji = "‚¯";  break;
				case 0x93: moji = "‚±";  break;
				case 0x94: moji = "‚³";  break;
				case 0x95: moji = "‚µ";  break;
				case 0x96: moji = "‚·";  break;
				case 0x97: moji = "‚¹";  break;
				case 0x98: moji = "‚»";  break;
				case 0x99: moji = "‚½";  break;
				case 0x9A: moji = "‚¿";  break;
				case 0x9B: moji = "‚Â";  break;
				case 0x9C: moji = "‚Ä";  break;
				case 0x9D: moji = "‚Æ";  break;
				case 0x9E: moji = "‚È";  break;
				case 0x9F: moji = "‚É";  break;
				case 0xA0: moji = "‚Ê";  break;
				case 0xA1: moji = "‚Ë";  break;
				case 0xA2: moji = "‚Ì";  break;
				case 0xA3: moji = "‚Í";  break;
				case 0xA4: moji = "‚Ð";  break;
				case 0xA5: moji = "‚Ó";  break;
				case 0xA6: moji = "‚Ö";  break;
				case 0xA7: moji = "‚Ù";  break;
				case 0xA8: moji = "‚Ü";  break;
				case 0xA9: moji = "‚Ý";  break;
				case 0xAA: moji = "‚Þ";  break;
				case 0xAB: moji = "‚ß";  break;
				case 0xAC: moji = "‚à";  break;
				case 0xAD: moji = "‚â";  break;
				case 0xAE: moji = "‚ä";  break;
				case 0xAF: moji = "‚æ";  break;
				case 0xB0: moji = "‚ç";  break;
				case 0xB1: moji = "‚è";  break;
				case 0xB2: moji = "‚é";  break;
				case 0xB3: moji = "‚ê";  break;
				case 0xB4: moji = "‚ë";  break;
				case 0xB5: moji = "‚í";  break;
				case 0xB6: moji = "‚ñ";  break;
				case 0xB7: moji = "‚ð";  break;
				case 0xB8: moji = "‚Á";  break;
				case 0xB9: moji = "‚á";  break;
				case 0xBA: moji = "‚ã";  break;
				case 0xBB: moji = "‚å";  break;
				case 0xBC: moji = "‚Ÿ";  break;
				case 0xBD: moji = "‚¡";  break;
				case 0xBE: moji = "‚£";  break;
				case 0xBF: moji = "‚¥";  break;
				case 0xC0: moji = "‚§";  break;
				case 0xC1: moji = "‚ª";  break;
				case 0xC2: moji = "‚¬";  break;
				case 0xC3: moji = "‚®";  break;
				case 0xC4: moji = "‚°";  break;
				case 0xC5: moji = "‚²";  break;
				case 0xC6: moji = "‚´";  break;
				case 0xC7: moji = "‚¶";  break;
				case 0xC8: moji = "‚¸";  break;
				case 0xC9: moji = "‚º";  break;
				case 0xCA: moji = "‚¼";  break;
				case 0xCB: moji = "‚¾";  break;
				case 0xCC: moji = "‚À";  break;
				case 0xCD: moji = "‚Ã";  break;
				case 0xCE: moji = "‚Å";  break;
				case 0xCF: moji = "‚Ç";  break;
				case 0xD0: moji = "‚Î";  break;
				case 0xD1: moji = "‚Ñ";  break;
				case 0xD2: moji = "‚Ô";  break;
				case 0xD3: moji = "‚×";  break;
				case 0xD4: moji = "‚Ú";  break;
				case 0xD5: moji = "‚Ï";  break;
				case 0xD6: moji = "‚Ò";  break;
				case 0xD7: moji = "‚Õ";  break;
				case 0xD8: moji = "‚Ø";  break;
				case 0xD9: moji = "‚Û";  break;
				case 0xDA: moji = "‚`";  break;
				case 0xDB: moji = "‚a";  break;
				case 0xDC: moji = "‚b";  break;
				case 0xDD: moji = "‚c";  break;
				case 0xDE: moji = "‚d";  break;
				case 0xDF: moji = "‚e";  break;
				case 0xE0: moji = "‚f";  break;
				case 0xE1: moji = "‚g";  break;
				case 0xE2: moji = "‚h";  break;
				case 0xE3: moji = "‚i";  break;
				case 0xE4: moji = "‚j";  break;
				case 0xE5: moji = "‚k";  break;
				case 0xE6: moji = "‚l";  break;
				case 0xE7: moji = "‚m";  break;
				case 0xE8: moji = "‚n";  break;
				case 0xE9: moji = "‚o";  break;
				case 0xEA: moji = "‚p";  break;
				case 0xEB: moji = "‚q";  break;
				case 0xEC: moji = "‚r";  break;
				case 0xED: moji = "‚s";  break;
				case 0xEE: moji = "‚t";  break;
				case 0xEF: moji = "‚u";  break;
				case 0xF0: moji = "‚v";  break;
				case 0xF1: moji = "‚w";  break;
				case 0xF2: moji = "‚x";  break;
				case 0xF3: moji = "‚y";  break;
				case 0xF4: moji = "E";  break;
				case 0xF5: moji = "F";  break;
				case 0xF6: moji = "G";  break;
				case 0xF7: moji = "`";  break;
				case 0xF8: moji = "^";  break;
				case 0xF9: moji = "i";  break;
				case 0xFA: moji = "j";  break;
				case 0xFB: moji = "¹";  break;
				case 0xFC: moji = "Œ•";  break;
				case 0xFD: moji = "“`";  break;
				case 0xFE: moji = "à";  break;
				case 0xFF: moji = "@";  break;
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
					case 0x0: moji = "A";  break;
					case 0x1: moji = "B";  break;
					case 0x2: moji = "I";  break;
					case 0x3: moji = "H";  break;
					case 0x4: moji = "[";  break;
					case 0x5: moji = "d";  break;
					case 0x6: moji = "u";  break;
					case 0x7: moji = "v";  break;
					case 0x8: moji = "w";  break;
					case 0x9: moji = "x";  break;
					case 0x0A: moji = "‚ ";  break;
					case 0x0B: moji = "‚¢";  break;
					case 0x0C: moji = "‚¤";  break;
					case 0x0D: moji = "‚¦";  break;
					case 0x0E: moji = "‚¨";  break;
					case 0x0F: moji = "‚©";  break;
					case 0x10: moji = "‚«";  break;
					case 0x11: moji = "‚­";  break;
					case 0x12: moji = "‚¯";  break;
					case 0x13: moji = "‚±";  break;
					case 0x14: moji = "‚³";  break;
					case 0x15: moji = "‚µ";  break;
					case 0x16: moji = "‚·";  break;
					case 0x17: moji = "‚¹";  break;
					case 0x18: moji = "‚»";  break;
					case 0x19: moji = "‚½";  break;
					case 0x1A: moji = "‚¿";  break;
					case 0x1B: moji = "‚Â";  break;
					case 0x1C: moji = "‚Ä";  break;
					case 0x1D: moji = "‚Æ";  break;
					case 0x1E: moji = "‚È";  break;
					case 0x1F: moji = "‚É";  break;
					case 0x20: moji = "‚Ê";  break;
					case 0x21: moji = "‚Ë";  break;
					case 0x22: moji = "‚Ì";  break;
					case 0x23: moji = "‚Í";  break;
					case 0x24: moji = "‚Ð";  break;
					case 0x25: moji = "‚Ó";  break;
					case 0x26: moji = "‚Ö";  break;
					case 0x27: moji = "‚Ù";  break;
					case 0x28: moji = "‚Ü";  break;
					case 0x29: moji = "‚Ý";  break;
					case 0x2A: moji = "‚Þ";  break;
					case 0x2B: moji = "‚ß";  break;
					case 0x2C: moji = "‚à";  break;
					case 0x2D: moji = "‚â";  break;
					case 0x2E: moji = "‚ä";  break;
					case 0x2F: moji = "‚æ";  break;
					case 0x30: moji = "‚ç";  break;
					case 0x31: moji = "‚è";  break;
					case 0x32: moji = "‚é";  break;
					case 0x33: moji = "‚ê";  break;
					case 0x34: moji = "‚ë";  break;
					case 0x35: moji = "‚í";  break;
					case 0x36: moji = "‚ñ";  break;
					case 0x37: moji = "‚ð";  break;
					case 0x38: moji = "‚Á";  break;
					case 0x39: moji = "‚á";  break;
					case 0x3A: moji = "‚ã";  break;
					case 0x3B: moji = "‚å";  break;
					case 0x3C: moji = "‚Ÿ";  break;
					case 0x3D: moji = "‚¡";  break;
					case 0x3E: moji = "‚£";  break;
					case 0x3F: moji = "‚¥";  break;
					case 0x40: moji = "‚§";  break;
					case 0x41: moji = "‚ª";  break;
					case 0x42: moji = "‚¬";  break;
					case 0x43: moji = "‚®";  break;
					case 0x44: moji = "‚°";  break;
					case 0x45: moji = "‚²";  break;
					case 0x46: moji = "‚´";  break;
					case 0x47: moji = "‚¶";  break;
					case 0x48: moji = "‚¸";  break;
					case 0x49: moji = "‚º";  break;
					case 0x4A: moji = "‚¼";  break;
					case 0x4B: moji = "‚¾";  break;
					case 0x4C: moji = "‚À";  break;
					case 0x4D: moji = "‚Ã";  break;
					case 0x4E: moji = "‚Å";  break;
					case 0x4F: moji = "‚Ç";  break;
					case 0x50: moji = "‚Î";  break;
					case 0x51: moji = "‚Ñ";  break;
					case 0x52: moji = "‚Ô";  break;
					case 0x53: moji = "‚×";  break;
					case 0x54: moji = "‚Ú";  break;
					case 0x55: moji = "‚Ï";  break;
					case 0x56: moji = "‚Ò";  break;
					case 0x57: moji = "‚Õ";  break;
					case 0x58: moji = "‚Ø";  break;
					case 0x59: moji = "‚Û";  break;
					case 0x5A: moji = "‚`";  break;
					case 0x5B: moji = "‚a";  break;
					case 0x5C: moji = "‚b";  break;
					case 0x5D: moji = "‚c";  break;
					case 0x5E: moji = "‚d";  break;
					case 0x5F: moji = "‚e";  break;
					case 0x60: moji = "‚f";  break;
					case 0x61: moji = "‚g";  break;
					case 0x62: moji = "‚h";  break;
					case 0x63: moji = "‚i";  break;
					case 0x64: moji = "‚j";  break;
					case 0x65: moji = "‚k";  break;
					case 0x66: moji = "‚l";  break;
					case 0x67: moji = "‚m";  break;
					case 0x68: moji = "‚n";  break;
					case 0x69: moji = "‚o";  break;
					case 0x6A: moji = "‚p";  break;
					case 0x6B: moji = "‚q";  break;
					case 0x6C: moji = "‚r";  break;
					case 0x6D: moji = "‚s";  break;
					case 0x6E: moji = "‚t";  break;
					case 0x6F: moji = "‚u";  break;
					case 0x70: moji = "‚v";  break;
					case 0x71: moji = "‚w";  break;
					case 0x72: moji = "‚x";  break;
					case 0x73: moji = "‚y";  break;
					case 0x74: moji = "E";  break;
					case 0x75: moji = "F";  break;
					case 0x76: moji = "G";  break;
					case 0x77: moji = "`";  break;
					case 0x78: moji = "^";  break;
					case 0x79: moji = "i";  break;
					case 0x7A: moji = "j";  break;
					case 0x7B: moji = "¹";  break;
					case 0x7C: moji = "Œ•";  break;
					case 0x7D: moji = "“`";  break;
					case 0x7E: moji = "à";  break;
					case 0x7F: moji = "@";  break;
					case 0x80: moji = "‚O";  break;
					case 0x81: moji = "‚P";  break;
					case 0x82: moji = "‚Q";  break;
					case 0x83: moji = "‚R";  break;
					case 0x84: moji = "‚S";  break;
					case 0x85: moji = "‚T";  break;
					case 0x86: moji = "‚U";  break;
					case 0x87: moji = "‚V";  break;
					case 0x88: moji = "‚W";  break;
					case 0x89: moji = "‚X";  break;
					case 0x8A: moji = "ƒA";  break;
					case 0x8B: moji = "ƒC";  break;
					case 0x8C: moji = "ƒE";  break;
					case 0x8D: moji = "ƒG";  break;
					case 0x8E: moji = "ƒI";  break;
					case 0x8F: moji = "ƒJ";  break;
					case 0x90: moji = "ƒL";  break;
					case 0x91: moji = "ƒN";  break;
					case 0x92: moji = "ƒP";  break;
					case 0x93: moji = "ƒR";  break;
					case 0x94: moji = "ƒT";  break;
					case 0x95: moji = "ƒV";  break;
					case 0x96: moji = "ƒX";  break;
					case 0x97: moji = "ƒZ";  break;
					case 0x98: moji = "ƒ\\";  break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x99: moji = "ƒ^";  break;
					case 0x9A: moji = "ƒ`";  break;
					case 0x9B: moji = "ƒc";  break;
					case 0x9C: moji = "ƒe";  break;
					case 0x9D: moji = "ƒg";  break;
					case 0x9E: moji = "ƒi";  break;
					case 0x9F: moji = "ƒj";  break;
					case 0xA0: moji = "ƒk";  break;
					case 0xA1: moji = "ƒl";  break;
					case 0xA2: moji = "ƒm";  break;
					case 0xA3: moji = "ƒn";  break;
					case 0xA4: moji = "ƒq";  break;
					case 0xA5: moji = "ƒt";  break;
					case 0xA6: moji = "ƒw";  break;
					case 0xA7: moji = "ƒz";  break;
					case 0xA8: moji = "ƒ}";  break;
					case 0xA9: moji = "ƒ~";  break;
					case 0xAA: moji = "ƒ€";  break;
					case 0xAB: moji = "ƒ";  break;
					case 0xAC: moji = "ƒ‚";  break;
					case 0xAD: moji = "ƒ„";  break;
					case 0xAE: moji = "ƒ†";  break;
					case 0xAF: moji = "ƒˆ";  break;
					case 0xB0: moji = "ƒ‰";  break;
					case 0xB1: moji = "ƒŠ";  break;
					case 0xB2: moji = "ƒ‹";  break;
					case 0xB3: moji = "ƒŒ";  break;
					case 0xB4: moji = "ƒ";  break;
					case 0xB5: moji = "ƒ";  break;
					case 0xB6: moji = "ƒ“";  break;
					case 0xB7: moji = "ƒ’";  break;
					case 0xB8: moji = "ƒb";  break;
					case 0xB9: moji = "ƒƒ";  break;
					case 0xBA: moji = "ƒ…";  break;
					case 0xBB: moji = "ƒ‡";  break;
					case 0xBC: moji = "ƒ@";  break;
					case 0xBD: moji = "ƒB";  break;
					case 0xBE: moji = "ƒD";  break;
					case 0xBF: moji = "ƒF";  break;
					case 0xC0: moji = "ƒH";  break;
					case 0xC1: moji = "ƒK";  break;
					case 0xC2: moji = "ƒM";  break;
					case 0xC3: moji = "ƒO";  break;
					case 0xC4: moji = "ƒQ";  break;
					case 0xC5: moji = "ƒS";  break;
					case 0xC6: moji = "ƒU";  break;
					case 0xC7: moji = "ƒW";  break;
					case 0xC8: moji = "ƒY";  break;
					case 0xC9: moji = "ƒ[";  break;
					case 0xCA: moji = "ƒ]";  break;
					case 0xCB: moji = "ƒ_";  break;
					case 0xCC: moji = "ƒa";  break;
					case 0xCD: moji = "ƒd";  break;
					case 0xCE: moji = "ƒf";  break;
					case 0xCF: moji = "ƒh";  break;
					case 0xD0: moji = "ƒo";  break;
					case 0xD1: moji = "ƒr";  break;
					case 0xD2: moji = "ƒu";  break;
					case 0xD3: moji = "ƒx";  break;
					case 0xD4: moji = "ƒ{";  break;
					case 0xD5: moji = "ƒp";  break;
					case 0xD6: moji = "ƒs";  break;
					case 0xD7: moji = "ƒv";  break;
					case 0xD8: moji = "ƒy";  break;
					case 0xD9: moji = "ƒ|";  break;
					case 0xDA: moji = "‚";  break;
					case 0xDB: moji = "‚‚";  break;
					case 0xDC: moji = "‚ƒ";  break;
					case 0xDD: moji = "‚„";  break;
					case 0xDE: moji = "‚…";  break;
					case 0xDF: moji = "‚†";  break;
					case 0xE0: moji = "‚‡";  break;
					case 0xE1: moji = "‚ˆ";  break;
					case 0xE2: moji = "‚‰";  break;
					case 0xE3: moji = "‚Š";  break;
					case 0xE4: moji = "‚‹";  break;
					case 0xE5: moji = "‚Œ";  break;
					case 0xE6: moji = "‚";  break;
					case 0xE7: moji = "‚Ž";  break;
					case 0xE8: moji = "‚";  break;
					case 0xE9: moji = "‚";  break;
					case 0xEA: moji = "‚‘";  break;
					case 0xEB: moji = "‚’";  break;
					case 0xEC: moji = "‚“";  break;
					case 0xED: moji = "‚”";  break;
					case 0xEE: moji = "‚•";  break;
					case 0xEF: moji = "‚–";  break;
					case 0xF0: moji = "‚—";  break;
					case 0xF1: moji = "‚˜";  break;
					case 0xF2: moji = "‚™";  break;
					case 0xF3: moji = "‚š";  break;
					case 0xF4: moji = "ƒ”";  break;
					case 0xF5: moji = "X";  break;
					case 0xF6: moji = "—Ž";  break;
					case 0xF7: moji = "ˆ«";  break;
					case 0xF8: moji = "“ü";  break;
					case 0xF9: moji = "Žè";  break;
					case 0xFA: moji = "—p";  break;
					case 0xFB: moji = "æ";  break;
					case 0xFC: moji = "ˆÀ";  break;
					case 0xFD: moji = "’N";  break;
					case 0xFE: moji = "‰¹";  break;
					case 0xFF: moji = "¤";  break;
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
					case 0x0: moji = "‚O";  break;
					case 0x1: moji = "‚P";  break;
					case 0x2: moji = "‚Q";  break;
					case 0x3: moji = "‚R";  break;
					case 0x4: moji = "‚S";  break;
					case 0x5: moji = "‚T";  break;
					case 0x6: moji = "‚U";  break;
					case 0x7: moji = "‚V";  break;
					case 0x8: moji = "‚W";  break;
					case 0x9: moji = "‚X";  break;
					case 0x0A: moji = "ƒA";  break;
					case 0x0B: moji = "ƒC";  break;
					case 0x0C: moji = "ƒE";  break;
					case 0x0D: moji = "ƒG";  break;
					case 0x0E: moji = "ƒI";  break;
					case 0x0F: moji = "ƒJ";  break;
					case 0x10: moji = "ƒL";  break;
					case 0x11: moji = "ƒN";  break;
					case 0x12: moji = "ƒP";  break;
					case 0x13: moji = "ƒR";  break;
					case 0x14: moji = "ƒT";  break;
					case 0x15: moji = "ƒV";  break;
					case 0x16: moji = "ƒX";  break;
					case 0x17: moji = "ƒZ";  break;
					case 0x18: moji = "ƒ\\";  break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x19: moji = "ƒ^";  break;
					case 0x1A: moji = "ƒ`";  break;
					case 0x1B: moji = "ƒc";  break;
					case 0x1C: moji = "ƒe";  break;
					case 0x1D: moji = "ƒg";  break;
					case 0x1E: moji = "ƒi";  break;
					case 0x1F: moji = "ƒj";  break;
					case 0x20: moji = "ƒk";  break;
					case 0x21: moji = "ƒl";  break;
					case 0x22: moji = "ƒm";  break;
					case 0x23: moji = "ƒn";  break;
					case 0x24: moji = "ƒq";  break;
					case 0x25: moji = "ƒt";  break;
					case 0x26: moji = "ƒw";  break;
					case 0x27: moji = "ƒz";  break;
					case 0x28: moji = "ƒ}";  break;
					case 0x29: moji = "ƒ~";  break;
					case 0x2A: moji = "ƒ€";  break;
					case 0x2B: moji = "ƒ";  break;
					case 0x2C: moji = "ƒ‚";  break;
					case 0x2D: moji = "ƒ„";  break;
					case 0x2E: moji = "ƒ†";  break;
					case 0x2F: moji = "ƒˆ";  break;
					case 0x30: moji = "ƒ‰";  break;
					case 0x31: moji = "ƒŠ";  break;
					case 0x32: moji = "ƒ‹";  break;
					case 0x33: moji = "ƒŒ";  break;
					case 0x34: moji = "ƒ";  break;
					case 0x35: moji = "ƒ";  break;
					case 0x36: moji = "ƒ“";  break;
					case 0x37: moji = "ƒ’";  break;
					case 0x38: moji = "ƒb";  break;
					case 0x39: moji = "ƒƒ";  break;
					case 0x3A: moji = "ƒ…";  break;
					case 0x3B: moji = "ƒ‡";  break;
					case 0x3C: moji = "ƒ@";  break;
					case 0x3D: moji = "ƒB";  break;
					case 0x3E: moji = "ƒD";  break;
					case 0x3F: moji = "ƒF";  break;
					case 0x40: moji = "ƒH";  break;
					case 0x41: moji = "ƒK";  break;
					case 0x42: moji = "ƒM";  break;
					case 0x43: moji = "ƒO";  break;
					case 0x44: moji = "ƒQ";  break;
					case 0x45: moji = "ƒS";  break;
					case 0x46: moji = "ƒU";  break;
					case 0x47: moji = "ƒW";  break;
					case 0x48: moji = "ƒY";  break;
					case 0x49: moji = "ƒ[";  break;
					case 0x4A: moji = "ƒ]";  break;
					case 0x4B: moji = "ƒ_";  break;
					case 0x4C: moji = "ƒa";  break;
					case 0x4D: moji = "ƒd";  break;
					case 0x4E: moji = "ƒf";  break;
					case 0x4F: moji = "ƒh";  break;
					case 0x50: moji = "ƒo";  break;
					case 0x51: moji = "ƒr";  break;
					case 0x52: moji = "ƒu";  break;
					case 0x53: moji = "ƒx";  break;
					case 0x54: moji = "ƒ{";  break;
					case 0x55: moji = "ƒp";  break;
					case 0x56: moji = "ƒs";  break;
					case 0x57: moji = "ƒv";  break;
					case 0x58: moji = "ƒy";  break;
					case 0x59: moji = "ƒ|";  break;
					case 0x5A: moji = "‚";  break;
					case 0x5B: moji = "‚‚";  break;
					case 0x5C: moji = "‚ƒ";  break;
					case 0x5D: moji = "‚„";  break;
					case 0x5E: moji = "‚…";  break;
					case 0x5F: moji = "‚†";  break;
					case 0x60: moji = "‚‡";  break;
					case 0x61: moji = "‚ˆ";  break;
					case 0x62: moji = "‚‰";  break;
					case 0x63: moji = "‚Š";  break;
					case 0x64: moji = "‚‹";  break;
					case 0x65: moji = "‚Œ";  break;
					case 0x66: moji = "‚";  break;
					case 0x67: moji = "‚Ž";  break;
					case 0x68: moji = "‚";  break;
					case 0x69: moji = "‚";  break;
					case 0x6A: moji = "‚‘";  break;
					case 0x6B: moji = "‚’";  break;
					case 0x6C: moji = "‚“";  break;
					case 0x6D: moji = "‚”";  break;
					case 0x6E: moji = "‚•";  break;
					case 0x6F: moji = "‚–";  break;
					case 0x70: moji = "‚—";  break;
					case 0x71: moji = "‚˜";  break;
					case 0x72: moji = "‚™";  break;
					case 0x73: moji = "‚š";  break;
					case 0x74: moji = "ƒ”";  break;
					case 0x75: moji = "X";  break;
					case 0x76: moji = "—Ž";  break;
					case 0x77: moji = "ˆ«";  break;
					case 0x78: moji = "“ü";  break;
					case 0x79: moji = "Žè";  break;
					case 0x7A: moji = "—p";  break;
					case 0x7B: moji = "æ";  break;
					case 0x7C: moji = "ˆÀ";  break;
					case 0x7D: moji = "’N";  break;
					case 0x7E: moji = "‰¹";  break;
					case 0x7F: moji = "¤";  break;
					case 0x80: moji = "Žå";  break;
					case 0x81: moji = "l";  break;
					case 0x82: moji = "’¬";  break;
					case 0x83: moji = "X";  break;
					case 0x84: moji = "—ˆ";  break;
					case 0x85: moji = "ŠÔ";  break;
					case 0x86: moji = "Žv";  break;
					case 0x87: moji = "‡";  break;
					case 0x88: moji = "Œõ";  break;
					case 0x89: moji = "Á";  break;
					case 0x8A: moji = "Œ`";  break;
					case 0x8B: moji = "‰ñ";  break;
					case 0x8C: moji = "¢";  break;
					case 0x8D: moji = "’·";  break;
					case 0x8E: moji = "˜V";  break;
					case 0x8F: moji = "“Œ";  break;
					case 0x90: moji = "¼";  break;
					case 0x91: moji = "—ð";  break;
					case 0x92: moji = "Žj";  break;
					case 0x93: moji = "‰ï";  break;
					case 0x94: moji = "ŠÖ";  break;
					case 0x95: moji = "ŒW";  break;
					case 0x96: moji = "^";  break;
					case 0x97: moji = "–k";  break;
					case 0x98: moji = "“ì";  break;
					case 0x99: moji = "‘å";  break;
					case 0x9A: moji = "’n";  break;
					case 0x9B: moji = "Î";  break;
					case 0x9C: moji = "–Ú";  break;
					case 0x9D: moji = "‰½";  break;
					case 0x9E: moji = "—Ñ";  break;
					case 0x9F: moji = "’Ê";  break;
					case 0xA0: moji = "’j";  break;
					case 0xA1: moji = "Œã";  break;
					case 0xA2: moji = "“X";  break;
					case 0xA3: moji = "–º";  break;
					case 0xA4: moji = "—";  break;
					case 0xA5: moji = "‹Ÿ";  break;
					case 0xA6: moji = "’é";  break;
					case 0xA7: moji = "‘";  break;
					case 0xA8: moji = "Ž„";  break;
					case 0xA9: moji = "‹q";  break;
					case 0xAA: moji = "‰»";  break;
					case 0xAB: moji = "s";  break;
					case 0xAC: moji = "•û";  break;
					case 0xAD: moji = "–¾";  break;
					case 0xAE: moji = "c";  break;
					case 0xAF: moji = "—v";  break;
					case 0xB0: moji = "Ç";  break;
					case 0xB1: moji = "‰J";  break;
					case 0xB2: moji = "–Ø";  break;
					case 0xB3: moji = "–Å";  break;
					case 0xB4: moji = "ŒN";  break;
					case 0xB5: moji = "“";  break;
					case 0xB6: moji = "¶";  break;
					case 0xB7: moji = "ŒŒ";  break;
					case 0xB8: moji = "²";  break;
					case 0xB9: moji = "Žç";  break;
					case 0xBA: moji = "í";  break;
					case 0xBB: moji = "‘ˆ";  break;
					case 0xBC: moji = "‹R";  break;
					case 0xBD: moji = "Žm";  break;
					case 0xBE: moji = "“G";  break;
					case 0xBF: moji = "ŒŠ";  break;
					case 0xC0: moji = "Œü";  break;
					case 0xC1: moji = "ˆø";  break;
					case 0xC2: moji = "Ž×";  break;
					case 0xC3: moji = "“®";  break;
					case 0xC4: moji = "¬";  break;
					case 0xC5: moji = "Œ÷";  break;
					case 0xC6: moji = "•v";  break;
					case 0xC7: moji = "•¶";  break;
					case 0xC8: moji = "Žá";  break;
					case 0xC9: moji = "—¬";  break;
					case 0xCA: moji = "Ž¸";  break;
					case 0xCB: moji = "…";  break;
					case 0xCC: moji = "ŠO";  break;
					case 0xCD: moji = "Ž©";  break;
					case 0xCE: moji = "•ª";  break;
					case 0xCF: moji = "g";  break;
					case 0xD0: moji = "–{";  break;
					case 0xD1: moji = "Žã";  break;
					case 0xD2: moji = "Ž¸";  break;
					case 0xD3: moji = "ŠB";  break;
					case 0xD4: moji = "Œ«";  break;
					case 0xD5: moji = "ŽÒ";  break;
					case 0xD6: moji = "ŠC";  break;
					case 0xD7: moji = "“‡";  break;
					case 0xD8: moji = "˜b";  break;
					case 0xD9: moji = "ŠE";  break;
					case 0xDA: moji = "‘S";  break;
					case 0xDB: moji = "Ž€";  break;
					case 0xDC: moji = "D";  break;
					case 0xDD: moji = "Ð";  break;
					case 0xDE: moji = "ŒÃ";  break;
					case 0xDF: moji = "‘ã";  break;
					case 0xE0: moji = "Šâ";  break;
					case 0xE1: moji = "•ó";  break;
					case 0xE2: moji = "•ƒ";  break;
					case 0xE3: moji = "”N";  break;
					case 0xE4: moji = "“a";  break;
					case 0xE5: moji = "‘";  break;
					case 0xE6: moji = "‚";  break;
					case 0xE7: moji = "‹­";  break;
					case 0xE8: moji = "Ø";  break;
					case 0xE9: moji = "’‡";  break;
					case 0xEA: moji = "Œn";  break;
					case 0xEB: moji = "”’";  break;
					case 0xEC: moji = "“Å";  break;
					case 0xED: moji = "Ž_";  break;
					case 0xEE: moji = "•¨";  break;
					case 0xEF: moji = "•œ";  break;
					case 0xF0: moji = "“_";  break;
					case 0xF1: moji = "–°";  break;
					case 0xF2: moji = "‰F";  break;
					case 0xF3: moji = "’ˆ";  break;
					case 0xF4: moji = "—¤";  break;
					case 0xF5: moji = "­";  break;
					case 0xF6: moji = "ŽR";  break;
					case 0xF7: moji = "Žw";  break;
					case 0xF8: moji = "‘ê";  break;
					case 0xF9: moji = "Ž–";  break;
					case 0xFA: moji = "“–";  break;
					case 0xFB: moji = "MP";  break;
					case 0xFC: moji = "ª";  break;
					case 0xFD: moji = "«";  break;
					case 0xFE: moji = "©";  break;
					case 0xFF: moji = "¨";  break;
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
					case 0x0: moji = "Žå";  break;
					case 0x1: moji = "l";  break;
					case 0x2: moji = "’¬";  break;
					case 0x3: moji = "X";  break;
					case 0x4: moji = "—ˆ";  break;
					case 0x5: moji = "ŠÔ";  break;
					case 0x6: moji = "Žv";  break;
					case 0x7: moji = "‡";  break;
					case 0x8: moji = "Œõ";  break;
					case 0x9: moji = "Á";  break;
					case 0x0A: moji = "Œ`";  break;
					case 0x0B: moji = "‰ñ";  break;
					case 0x0C: moji = "¢";  break;
					case 0x0D: moji = "’·";  break;
					case 0x0E: moji = "˜V";  break;
					case 0x0F: moji = "“Œ";  break;
					case 0x10: moji = "¼";  break;
					case 0x11: moji = "—ð";  break;
					case 0x12: moji = "Žj";  break;
					case 0x13: moji = "‰ï";  break;
					case 0x14: moji = "ŠÖ";  break;
					case 0x15: moji = "ŒW";  break;
					case 0x16: moji = "^";  break;
					case 0x17: moji = "–k";  break;
					case 0x18: moji = "“ì";  break;
					case 0x19: moji = "‘å";  break;
					case 0x1A: moji = "’n";  break;
					case 0x1B: moji = "Î";  break;
					case 0x1C: moji = "–Ú";  break;
					case 0x1D: moji = "‰½";  break;
					case 0x1E: moji = "—Ñ";  break;
					case 0x1F: moji = "’Ê";  break;
					case 0x20: moji = "’j";  break;
					case 0x21: moji = "Œã";  break;
					case 0x22: moji = "“X";  break;
					case 0x23: moji = "–º";  break;
					case 0x24: moji = "—";  break;
					case 0x25: moji = "‹Ÿ";  break;
					case 0x26: moji = "’é";  break;
					case 0x27: moji = "‘";  break;
					case 0x28: moji = "Ž„";  break;
					case 0x29: moji = "‹q";  break;
					case 0x2A: moji = "‰»";  break;
					case 0x2B: moji = "s";  break;
					case 0x2C: moji = "•û";  break;
					case 0x2D: moji = "–¾";  break;
					case 0x2E: moji = "c";  break;
					case 0x2F: moji = "—v";  break;
					case 0x30: moji = "Ç";  break;
					case 0x31: moji = "‰J";  break;
					case 0x32: moji = "–Ø";  break;
					case 0x33: moji = "–Å";  break;
					case 0x34: moji = "ŒN";  break;
					case 0x35: moji = "“";  break;
					case 0x36: moji = "¶";  break;
					case 0x37: moji = "ŒŒ";  break;
					case 0x38: moji = "²";  break;
					case 0x39: moji = "Žç";  break;
					case 0x3A: moji = "í";  break;
					case 0x3B: moji = "‘ˆ";  break;
					case 0x3C: moji = "‹R";  break;
					case 0x3D: moji = "Žm";  break;
					case 0x3E: moji = "“G";  break;
					case 0x3F: moji = "ŒŠ";  break;
					case 0x40: moji = "Œü";  break;
					case 0x41: moji = "ˆø";  break;
					case 0x42: moji = "Ž×";  break;
					case 0x43: moji = "“®";  break;
					case 0x44: moji = "¬";  break;
					case 0x45: moji = "Œ÷";  break;
					case 0x46: moji = "•v";  break;
					case 0x47: moji = "•¶";  break;
					case 0x48: moji = "Žá";  break;
					case 0x49: moji = "—¬";  break;
					case 0x4A: moji = "Ž¸";  break;
					case 0x4B: moji = "…";  break;
					case 0x4C: moji = "ŠO";  break;
					case 0x4D: moji = "Ž©";  break;
					case 0x4E: moji = "•ª";  break;
					case 0x4F: moji = "g";  break;
					case 0x50: moji = "–{";  break;
					case 0x51: moji = "Žã";  break;
					case 0x52: moji = "Ž¸";  break;
					case 0x53: moji = "ŠB";  break;
					case 0x54: moji = "Œ«";  break;
					case 0x55: moji = "ŽÒ";  break;
					case 0x56: moji = "ŠC";  break;
					case 0x57: moji = "“‡";  break;
					case 0x58: moji = "˜b";  break;
					case 0x59: moji = "ŠE";  break;
					case 0x5A: moji = "‘S";  break;
					case 0x5B: moji = "Ž€";  break;
					case 0x5C: moji = "D";  break;
					case 0x5D: moji = "Ð";  break;
					case 0x5E: moji = "ŒÃ";  break;
					case 0x5F: moji = "‘ã";  break;
					case 0x60: moji = "Šâ";  break;
					case 0x61: moji = "•ó";  break;
					case 0x62: moji = "•ƒ";  break;
					case 0x63: moji = "”N";  break;
					case 0x64: moji = "“a";  break;
					case 0x65: moji = "‘";  break;
					case 0x66: moji = "‚";  break;
					case 0x67: moji = "‹­";  break;
					case 0x68: moji = "Ø";  break;
					case 0x69: moji = "’‡";  break;
					case 0x6A: moji = "Œn";  break;
					case 0x6B: moji = "”’";  break;
					case 0x6C: moji = "“Å";  break;
					case 0x6D: moji = "Ž_";  break;
					case 0x6E: moji = "•¨";  break;
					case 0x6F: moji = "•œ";  break;
					case 0x70: moji = "“_";  break;
					case 0x71: moji = "–°";  break;
					case 0x72: moji = "‰F";  break;
					case 0x73: moji = "’ˆ";  break;
					case 0x74: moji = "—¤";  break;
					case 0x75: moji = "­";  break;
					case 0x76: moji = "ŽR";  break;
					case 0x77: moji = "Žw";  break;
					case 0x78: moji = "‘ê";  break;
					case 0x79: moji = "Ž–";  break;
					case 0x7A: moji = "“–";  break;
					case 0x7B: moji = "MP";  break;
					case 0x7C: moji = "ª";  break;
					case 0x7D: moji = "«";  break;
					case 0x7E: moji = "©";  break;
					case 0x7F: moji = "¨";  break;
					case 0x80: moji = "‰Ÿ";  break;
					case 0x81: moji = "ˆÈ";  break;
					case 0x82: moji = "é";  break;
					case 0x83: moji = "Žš";  break;
					case 0x84: moji = "\\";  break;//more wierdness
					case 0x85: moji = "Œˆ";  break;
					case 0x86: moji = "‘I";  break;
					case 0x87: moji = "’è";  break;
					case 0x88: moji = "Žž";  break;
					case 0x89: moji = "¶";  break;
					case 0x8A: moji = "‰E";  break;
					case 0x8B: moji = "i";  break;
					case 0x8C: moji = "ŽŸ";  break;
					case 0x8D: moji = "–ß";  break;
					case 0x8E: moji = "–‚";  break;
					case 0x8F: moji = "Œ©";  break;
					case 0x90: moji = "–@";  break;
					case 0x91: moji = "—Í";  break;
					case 0x92: moji = "l";  break;
					case 0x93: moji = "Ší";  break;
					case 0x94: moji = "”ð";  break;
					case 0x95: moji = "•";  break;
					case 0x96: moji = "•Ï";  break;
					case 0x97: moji = "Še";  break;
					case 0x98: moji = "U";  break;
					case 0x99: moji = "Žn";  break;
					case 0x9A: moji = "Œ‚";  break;
					case 0x9B: moji = "Á";  break;
					case 0x9C: moji = "‰ñ";  break;
					case 0x9D: moji = "I";  break;
					case 0x9E: moji = "–h";  break;
					case 0x9F: moji = "Œä";  break;
					case 0xA0: moji = "‰®";  break;
					case 0xA1: moji = "ˆê";  break;
					case 0xA2: moji = "Œ»";  break;
					case 0xA3: moji = "Å";  break;
					case 0xA4: moji = "‰";  break;
					case 0xA5: moji = "“¯";  break;
					case 0xA6: moji = "ã";  break;
					case 0xA7: moji = "‰º";  break;
					case 0xA8: moji = "‹Z";  break;
					case 0xA9: moji = "–Ê";  break;
					case 0xAA: moji = "‹¤";  break;
					case 0xAB: moji = "—·";  break;
					case 0xAC: moji = "–Â";  break;
					case 0xAD: moji = "•½";  break;
					case 0xAE: moji = "¸";  break;
					case 0xAF: moji = "˜a";  break;
					case 0xB0: moji = "’m";  break;
					case 0xB1: moji = "‘Ì";  break;
					case 0xB2: moji = "•X";  break;
					case 0xB3: moji = "‘D";  break;
					case 0xB4: moji = "’†";  break;
					case 0xB5: moji = "_";  break;
					case 0xB6: moji = "–½";  break;
					case 0xB7: moji = "‘O";  break;
					case 0xB8: moji = "¬";  break;
					case 0xB9: moji = "‘½";  break;
					case 0xBA: moji = "¢";  break;
					case 0xBB: moji = "ì";  break;
					case 0xBC: moji = "‘±";  break;
					case 0xBD: moji = "ŠE";  break;
					case 0xBE: moji = "b";  break;
					case 0xBF: moji = "‹à";  break;
					case 0xC0: moji = "‹@";  break;
					case 0xC1: moji = "Ú";  break;
					case 0xC2: moji = "‹ß";  break;
					case 0xC3: moji = "‘Ò";  break;
					case 0xC4: moji = "–¼";  break;
					case 0xC5: moji = "—E";  break;
					case 0xC6: moji = "‹C";  break;
					case 0xC7: moji = "ˆ¤";  break;
					case 0xC8: moji = "Œb";  break;
					case 0xC9: moji = "S";  break;
					case 0xCA: moji = "»";  break;
					case 0xCB: moji = "”™";  break;
					case 0xCC: moji = "’ê";  break;
					case 0xCD: moji = "Ì";  break;
					case 0xCE: moji = "U";  break;
					case 0xCF: moji = "‹N";  break;
					case 0xD0: moji = "“";  break;
					case 0xD1: moji = "‰Î";  break;
					case 0xD2: moji = "‰Š";  break;
					case 0xD3: moji = "—³";  break;
					case 0xD4: moji = "“V";  break;
					case 0xD5: moji = "‰©";  break;
					case 0xD6: moji = "‹¹";  break;
					case 0xD7: moji = "Žg";  break;
					case 0xD8: moji = "ˆÅ";  break;
					case 0xD9: moji = "“¹";  break;
					case 0xDA: moji = "’…";  break;
					case 0xDB: moji = "Šv";  break;
					case 0xDC: moji = "‰¤";  break;
					case 0xDD: moji = "Š¥";  break;
					case 0xDE: moji = "—ì";  break;
					case 0xDF: moji = "‰H";  break;
					case 0xE0: moji = "•ê";  break;
					case 0xE1: moji = "”²";  break;
					case 0xE2: moji = "o";  break;
					case 0xE3: moji = "‘º";  break;
					case 0xE4: moji = "Žq";  break;
					case 0xE5: moji = "–C";  break;
					case 0xE6: moji = "”š";  break;
					case 0xE7: moji = "’e";  break;
					case 0xE8: moji = "j";  break;
					case 0xE9: moji = "”t";  break;
					case 0xEA: moji = "–ò";  break;
					case 0xEB: moji = "•—";  break;
					case 0xEC: moji = "‘¾";  break;
					case 0xED: moji = "ŒÛ";  break;
					case 0xEE: moji = "Ží";  break;
					case 0xEF: moji = "” ";  break;
					case 0xF0: moji = "–¡";  break;
					case 0xF1: moji = "— ";  break;
					case 0xF2: moji = "ŠÛ";  break;
					case 0xF3: moji = "ŒŽ";  break;
					case 0xF4: moji = "—Ö";  break;
					case 0xF5: moji = "‹â";  break;
					case 0xF6: moji = "–î";  break;
					case 0xF7: moji = "‹|";  break;
					case 0xF8: moji = "“S";  break;
					case 0xF9: moji = "”\\";  break;//more wierdness
					case 0xFA: moji = "‹";  break;
					case 0xFB: moji = "—d";  break;
					case 0xFC: moji = "C";  break;
					case 0xFD: moji = "••";  break;
					case 0xFE: moji = "ˆó";  break;
					case 0xFF: moji = "•·";  break;
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
