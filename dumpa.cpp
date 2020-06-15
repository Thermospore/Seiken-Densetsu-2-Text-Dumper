#include<stdio.h>
#define DEBUG true

//TO-DO:
//goal: match appearance of in game textbox (bar debug output)
//check rest of stuff in badParses
//fix string coding stuff, so it doesn't take forever to compile...

//advances to next byte
void advance(unsigned char &cur, FILE* pROM, int &addr)
{
	fread(&cur, 1, 1, pROM); //read next byte
	addr++; //update current address
	return;
}

//Handles in game NewLines
void gNL(FILE* pDUMP, int &col, int &row)
{
	col = -1;
	if(row == 2)
	{
		//wraps to new textbox after the third row
		//would print address too, but don't want to add another param to the fn
		fprintf(pDUMP,DEBUG ? "\n<RWRAP>\n" : "\n\n");
		row = 0;
	}
	else
	{
		fprintf(pDUMP,"\n");
		row++;
	}
	return;
}

//Prints game text
void gPrint(char* moji, FILE* pDUMP, int &col, int &row)
{
	//wrap column
	if (col == 15)
	{
		fprintf(pDUMP,DEBUG ? "<CWRAP>" : "");
		gNL(pDUMP,col,row);
	}
	
	col++; //advance column
	fprintf(pDUMP,"%s",moji); //print char
	return;
}

//Sets column (by spitting out spaces)
void setCol(unsigned char cur, FILE* pDUMP, int &col, int &row)
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
				gPrint("Å@",pDUMP,col,row);
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
	char* moji = "ñ≥"; //moji to print
	int addr = -1; //address in ROM of `cur`
	int col = -1; //column the most recently printed charachchtter is in
	int row = 0; //current row of text you are printing on now
	
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
			fprintf(pDUMP,"<EOS@%0.6X>\n\n",addr);
			col = -1;
			row = 0;
		}
		//inn price reference?
		else if(cur == 0x02)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,"<REF?%0.2X>",cur);
		}
		//text stutter
		else if(cur == 0x08)
		{
			fprintf(pDUMP,DEBUG ? "<...>" : "");
		}
		//Unknown. Related to attack moves/rendering?
		//Actually, maybe to y/n selections?
		else if(cur == 0x05 || cur == 0x09)
		{
			fprintf(pDUMP,DEBUG ? "<%0.2X?>" : "",cur);
		}
		//Not sure. seems to be related to selection menus?
		else if(cur == 0x11)
		{
			//assuming it has two parameters for now, but I might be wrong...
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<11?%0.2X" : "",cur);
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "%0.2X>" : "",cur);
		}
		//Pause at end of dialogue, close window after a button press,
		//then restore control to charachchther
		else if(cur == 0x15)
		{
			advance(cur,pROM,addr);
			if(cur == 0xFF)
			{
				fprintf(pDUMP,"\n<RET@%0.6X>",addr);
			}
			else
			{
				fprintf(pDUMP,"<15?%0.2X>",cur);
			}
			col = -1;
			row = 0;
		}
		//no clue. found before recieving the bronze lance.
		else if(cur == 0x1E)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<1E?%0.2X>" : "",cur);
		}
		//ehhh idunno. Movement related?
		else if(cur == 0x20)
		{
			advance(cur,pROM,addr);
			//this makes him do a head shake tho
			if (cur == 0x5F)
			{
				fprintf(pDUMP,DEBUG ? "<HEADSHAKE>" : "");
			}
			else
			{
				fprintf(pDUMP,DEBUG ? "<20?%0.2X>" : "",cur);
			}
		}
		//more movement related stuff?
		else if(cur == 0x21)
		{
			advance(cur,pROM,addr);
			//this makes him turn left and right to look around
			if (cur == 0x2D)
			{
				fprintf(pDUMP,DEBUG ? "<LOOKAROUND>" : "");
			}
			else
			{
				fprintf(pDUMP,DEBUG ? "<21?%0.2X>" : "",cur);
			}
		}
		//without it the chars/camera wont't get in formation???
		//seen at start of shops???
		else if(cur == 0x24)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<FORM?%0.2X>" : "",cur);
		}
		//related to audio?
		else if(cur == 0x27)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<AUDIO?%0.2X>" : "",cur);
		}
		//pauses
		else if(cur == 0x28)
		{
			advance(cur,pROM,addr);
			//wait for button press
			if (cur == 0x00)
			{
				fprintf(pDUMP,DEBUG ? "<PAUSE>" : "");
			}
			//wait for an amount of time
			else
			{
				fprintf(pDUMP,DEBUG ? "<WAIT%0.2X>" : "",cur);
			}
		}
		//i aint got no clue
		else if(cur == 0x29)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<29?%0.2X>" : "",cur);
		}
		//again, no clue
		else if(cur == 0x2A)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<2A?%0.2X>" : "",cur);
		}
		//Something to to with screen fades? ie to white, to yellow, etc
		else if(cur == 0x2D)
		{
			advance(cur,pROM,addr);
			// 0x06 seems like a special case
			if(cur == 0x06)
			{
				fprintf(pDUMP,DEBUG ? "<FADE06?" : "",cur);
				
				//dump next two bytes
				advance(cur,pROM,addr);
				fprintf(pDUMP,DEBUG ? "%0.2X" : "",cur);
				advance(cur,pROM,addr);
				fprintf(pDUMP,DEBUG ? "%0.2X>" : "",cur);
			}
			else
			{
				fprintf(pDUMP,DEBUG ? "<FADE?%0.2X>" : "",cur);
			}
		}
		//unknown
		else if(cur == 0x30)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<30?%0.2X>" : "",cur);
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
				fprintf(pDUMP,DEBUG ? "<ATK>" : "",cur);
			}
			else
			{
				fprintf(pDUMP,DEBUG ? "<31?%0.2X>" : "",cur);	
			}				
		}
		//characchehter movement
		else if(cur == 0x32)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<MVE%0.2X>" : "",cur);
			advance(cur,pROM,addr);//dump movement control
		}
		//chahrahrer action?
		else if(cur == 0x34)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<ACT%0.2X?" : "",cur);
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "%0.2X>" : "",cur);
		}
		//item movement?
		else if(cur == 0x39)
		{
			advance(cur,pROM,addr);
			//these seem to be special cases that take more parameters.
			//actually it's possible all 0x39 take 3 params;
			//maybe I was just seeing an incomplete command that looked like
			//a 0x39 only taking 1 param... investigate later if it comes up
			if(cur <= 0x04 || cur == 0x80)
			{
				fprintf(pDUMP,DEBUG ? "<ITEM%0.2X?" : "",cur);
				advance(cur,pROM,addr);
				fprintf(pDUMP,DEBUG ? "%0.2X" : "",cur);
				advance(cur,pROM,addr);
				fprintf(pDUMP,DEBUG ? "%0.2X>" : "",cur);
			}
			else
			{
				fprintf(pDUMP,DEBUG ? "<ITEM?%0.2X>" : "",cur);
			}
		}
		//fancy cutscene stuff?
		else if(cur == 0x40)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<FX?%0.2X?" : "",cur);
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "%0.2X" : "",cur);
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "%0.2X" : "",cur);
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "%0.2X>" : "",cur);
		}
		//more cutscene stuff??
		else if(cur == 0x42)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<42?%0.2X" : "",cur);
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "%0.2X" : "",cur);
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "%0.2X" : "",cur);
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "%0.2X>" : "",cur);
		}
		//textbox open
		else if(cur == 0x50)
		{
			fprintf(pDUMP,"\n\n<OPEN@%0.6X>\n",addr);
			col = -1;
			row = 0;
		}
		//textbox close
		else if(cur == 0x51)
		{
			fprintf(pDUMP,"\n<CLOSE@%0.6X>\n\n",addr);
			col = -1;
			row = 0;
		}
		//textbox clear
		else if(cur == 0x52)
		{
			fprintf(pDUMP,DEBUG ? "\n<CLEAR@%0.6X>\n" : "\n\n",addr);
			col = -1;
			row = 0;
		}
		//charachthtere names
		else if(cur == 0x57)
		{
			advance(cur,pROM,addr);
			
			if(cur == 0x00)
			{
				//a little funky, I know, but it was the easiest
				//way to prevent wrapping issues
				gPrint("<ÉâÉìÉfÉB>",pDUMP,col,row);
				col+=3;
			} 
			else if(cur == 0x01)
			{
				gPrint("<ÉvÉäÉÄ>",pDUMP,col,row);
				col+=2;
			}
			else if(cur == 0x02)
			{
				gPrint("<É|É|ÉC>",pDUMP,col,row);
				col+=2;
			}
			else
			{
				fprintf(pDUMP,"<NAME%0.2X>",cur);
			}
		}
		//initialize a selection list?
		else if(cur == 0x58)
		{
			fprintf(pDUMP,DEBUG ? "<SEL>" : "");
		}
		//set current column
		//(this is independent of selection lists)
		else if(cur == 0x59)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<COL%X>" : "",cur);
			
			setCol(cur,pDUMP,col,row);			
		}
		//set the column/location for a selection?
		else if(cur == 0x5A)
		{
			advance(cur,pROM,addr);
			fprintf(pDUMP,DEBUG ? "<SCOL%X>" : "",cur);
			
			setCol(cur,pDUMP,col,row);	
		}
		//finalize a selection list?
		else if(cur == 0x5B)
		{
			fprintf(pDUMP,DEBUG ? "</SEL>\n" : "\n");
		}
		//bring up subtextbox (IE to display your money in a shop)
		//note: technically, if you use this code your cursor will move to the start of
		//the current textbox and any new text will overwrite the existing text.
		//though, it seems like the devs always(?) move to a new box beforehand.
		//this could be good to keep in mind if something wierd is going on 
		else if(cur == 0x5D)
		{
			fprintf(pDUMP,DEBUG ? "<SBOX>" : "");
			col = -1;
			row = 0;
		}
		//show current $  in subtextbox
		else if(cur == 0x5F)
		{
			fprintf(pDUMP,DEBUG ? "<$>" : "");
		}
		//newline
		else if(cur == 0x7F)
		{
			//this is a workaround for a discrepancy between the way
			//I handle line wrapping and the game handles line wrapping
			if(col == 15)
			{
				fprintf(pDUMP,DEBUG ? "<CWRAPNL>" : "");
				gNL(pDUMP,col,row);
			}
			
			fprintf(pDUMP,DEBUG ? "<NL@%0.6X>" : "",addr);
			gNL(pDUMP,col,row);
		}
		//no shift
		else if(cur >= 0x80 && cur <= 0xFF)
		{
			switch (cur)
			{
			    case 0x80: moji = "ÅA";  break;
				case 0x81: moji = "ÅB";  break;
				case 0x82: moji = "ÅI";  break;
				case 0x83: moji = "ÅH";  break;
				case 0x84: moji = "Å[";  break;
				case 0x85: moji = "Åd";  break;
				case 0x86: moji = "Åu";  break;
				case 0x87: moji = "Åv";  break;
				case 0x88: moji = "Åw";  break;
				case 0x89: moji = "Åx";  break;
				case 0x8A: moji = "Ç†";  break;
				case 0x8B: moji = "Ç¢";  break;
				case 0x8C: moji = "Ç§";  break;
				case 0x8D: moji = "Ç¶";  break;
				case 0x8E: moji = "Ç®";  break;
				case 0x8F: moji = "Ç©";  break;
				case 0x90: moji = "Ç´";  break;
				case 0x91: moji = "Ç≠";  break;
				case 0x92: moji = "ÇØ";  break;
				case 0x93: moji = "Ç±";  break;
				case 0x94: moji = "Ç≥";  break;
				case 0x95: moji = "Çµ";  break;
				case 0x96: moji = "Ç∑";  break;
				case 0x97: moji = "Çπ";  break;
				case 0x98: moji = "Çª";  break;
				case 0x99: moji = "ÇΩ";  break;
				case 0x9A: moji = "Çø";  break;
				case 0x9B: moji = "Ç¬";  break;
				case 0x9C: moji = "Çƒ";  break;
				case 0x9D: moji = "Ç∆";  break;
				case 0x9E: moji = "Ç»";  break;
				case 0x9F: moji = "Ç…";  break;
				case 0xA0: moji = "Ç ";  break;
				case 0xA1: moji = "ÇÀ";  break;
				case 0xA2: moji = "ÇÃ";  break;
				case 0xA3: moji = "ÇÕ";  break;
				case 0xA4: moji = "Ç–";  break;
				case 0xA5: moji = "Ç”";  break;
				case 0xA6: moji = "Ç÷";  break;
				case 0xA7: moji = "ÇŸ";  break;
				case 0xA8: moji = "Ç‹";  break;
				case 0xA9: moji = "Ç›";  break;
				case 0xAA: moji = "Çﬁ";  break;
				case 0xAB: moji = "Çﬂ";  break;
				case 0xAC: moji = "Ç‡";  break;
				case 0xAD: moji = "Ç‚";  break;
				case 0xAE: moji = "Ç‰";  break;
				case 0xAF: moji = "ÇÊ";  break;
				case 0xB0: moji = "ÇÁ";  break;
				case 0xB1: moji = "ÇË";  break;
				case 0xB2: moji = "ÇÈ";  break;
				case 0xB3: moji = "ÇÍ";  break;
				case 0xB4: moji = "ÇÎ";  break;
				case 0xB5: moji = "ÇÌ";  break;
				case 0xB6: moji = "ÇÒ";  break;
				case 0xB7: moji = "Ç";  break;
				case 0xB8: moji = "Ç¡";  break;
				case 0xB9: moji = "Ç·";  break;
				case 0xBA: moji = "Ç„";  break;
				case 0xBB: moji = "ÇÂ";  break;
				case 0xBC: moji = "Çü";  break;
				case 0xBD: moji = "Ç°";  break;
				case 0xBE: moji = "Ç£";  break;
				case 0xBF: moji = "Ç•";  break;
				case 0xC0: moji = "Çß";  break;
				case 0xC1: moji = "Ç™";  break;
				case 0xC2: moji = "Ç¨";  break;
				case 0xC3: moji = "ÇÆ";  break;
				case 0xC4: moji = "Ç∞";  break;
				case 0xC5: moji = "Ç≤";  break;
				case 0xC6: moji = "Ç¥";  break;
				case 0xC7: moji = "Ç∂";  break;
				case 0xC8: moji = "Ç∏";  break;
				case 0xC9: moji = "Ç∫";  break;
				case 0xCA: moji = "Çº";  break;
				case 0xCB: moji = "Çæ";  break;
				case 0xCC: moji = "Ç¿";  break;
				case 0xCD: moji = "Ç√";  break;
				case 0xCE: moji = "Ç≈";  break;
				case 0xCF: moji = "Ç«";  break;
				case 0xD0: moji = "ÇŒ";  break;
				case 0xD1: moji = "Ç—";  break;
				case 0xD2: moji = "Ç‘";  break;
				case 0xD3: moji = "Ç◊";  break;
				case 0xD4: moji = "Ç⁄";  break;
				case 0xD5: moji = "Çœ";  break;
				case 0xD6: moji = "Ç“";  break;
				case 0xD7: moji = "Ç’";  break;
				case 0xD8: moji = "Çÿ";  break;
				case 0xD9: moji = "Ç€";  break;
				case 0xDA: moji = "Ç`";  break;
				case 0xDB: moji = "Ça";  break;
				case 0xDC: moji = "Çb";  break;
				case 0xDD: moji = "Çc";  break;
				case 0xDE: moji = "Çd";  break;
				case 0xDF: moji = "Çe";  break;
				case 0xE0: moji = "Çf";  break;
				case 0xE1: moji = "Çg";  break;
				case 0xE2: moji = "Çh";  break;
				case 0xE3: moji = "Çi";  break;
				case 0xE4: moji = "Çj";  break;
				case 0xE5: moji = "Çk";  break;
				case 0xE6: moji = "Çl";  break;
				case 0xE7: moji = "Çm";  break;
				case 0xE8: moji = "Çn";  break;
				case 0xE9: moji = "Ço";  break;
				case 0xEA: moji = "Çp";  break;
				case 0xEB: moji = "Çq";  break;
				case 0xEC: moji = "Çr";  break;
				case 0xED: moji = "Çs";  break;
				case 0xEE: moji = "Çt";  break;
				case 0xEF: moji = "Çu";  break;
				case 0xF0: moji = "Çv";  break;
				case 0xF1: moji = "Çw";  break;
				case 0xF2: moji = "Çx";  break;
				case 0xF3: moji = "Çy";  break;
				case 0xF4: moji = "ÅE";  break;
				case 0xF5: moji = "ÅF";  break;
				case 0xF6: moji = "ÅG";  break;
				case 0xF7: moji = "Å`";  break;
				case 0xF8: moji = "Å^";  break;
				case 0xF9: moji = "Åi";  break;
				case 0xFA: moji = "Åj";  break;
				case 0xFB: moji = "êπ";  break;
				case 0xFC: moji = "åï";  break;
				case 0xFD: moji = "ì`";  break;
				case 0xFE: moji = "ê‡";  break;
				case 0xFF: moji = "Å@";  break;
			}
			gPrint(moji,pDUMP,col,row);
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
					case 0x0: moji = "ÅA";  break;
					case 0x1: moji = "ÅB";  break;
					case 0x2: moji = "ÅI";  break;
					case 0x3: moji = "ÅH";  break;
					case 0x4: moji = "Å[";  break;
					case 0x5: moji = "Åd";  break;
					case 0x6: moji = "Åu";  break;
					case 0x7: moji = "Åv";  break;
					case 0x8: moji = "Åw";  break;
					case 0x9: moji = "Åx";  break;
					case 0x0A: moji = "Ç†";  break;
					case 0x0B: moji = "Ç¢";  break;
					case 0x0C: moji = "Ç§";  break;
					case 0x0D: moji = "Ç¶";  break;
					case 0x0E: moji = "Ç®";  break;
					case 0x0F: moji = "Ç©";  break;
					case 0x10: moji = "Ç´";  break;
					case 0x11: moji = "Ç≠";  break;
					case 0x12: moji = "ÇØ";  break;
					case 0x13: moji = "Ç±";  break;
					case 0x14: moji = "Ç≥";  break;
					case 0x15: moji = "Çµ";  break;
					case 0x16: moji = "Ç∑";  break;
					case 0x17: moji = "Çπ";  break;
					case 0x18: moji = "Çª";  break;
					case 0x19: moji = "ÇΩ";  break;
					case 0x1A: moji = "Çø";  break;
					case 0x1B: moji = "Ç¬";  break;
					case 0x1C: moji = "Çƒ";  break;
					case 0x1D: moji = "Ç∆";  break;
					case 0x1E: moji = "Ç»";  break;
					case 0x1F: moji = "Ç…";  break;
					case 0x20: moji = "Ç ";  break;
					case 0x21: moji = "ÇÀ";  break;
					case 0x22: moji = "ÇÃ";  break;
					case 0x23: moji = "ÇÕ";  break;
					case 0x24: moji = "Ç–";  break;
					case 0x25: moji = "Ç”";  break;
					case 0x26: moji = "Ç÷";  break;
					case 0x27: moji = "ÇŸ";  break;
					case 0x28: moji = "Ç‹";  break;
					case 0x29: moji = "Ç›";  break;
					case 0x2A: moji = "Çﬁ";  break;
					case 0x2B: moji = "Çﬂ";  break;
					case 0x2C: moji = "Ç‡";  break;
					case 0x2D: moji = "Ç‚";  break;
					case 0x2E: moji = "Ç‰";  break;
					case 0x2F: moji = "ÇÊ";  break;
					case 0x30: moji = "ÇÁ";  break;
					case 0x31: moji = "ÇË";  break;
					case 0x32: moji = "ÇÈ";  break;
					case 0x33: moji = "ÇÍ";  break;
					case 0x34: moji = "ÇÎ";  break;
					case 0x35: moji = "ÇÌ";  break;
					case 0x36: moji = "ÇÒ";  break;
					case 0x37: moji = "Ç";  break;
					case 0x38: moji = "Ç¡";  break;
					case 0x39: moji = "Ç·";  break;
					case 0x3A: moji = "Ç„";  break;
					case 0x3B: moji = "ÇÂ";  break;
					case 0x3C: moji = "Çü";  break;
					case 0x3D: moji = "Ç°";  break;
					case 0x3E: moji = "Ç£";  break;
					case 0x3F: moji = "Ç•";  break;
					case 0x40: moji = "Çß";  break;
					case 0x41: moji = "Ç™";  break;
					case 0x42: moji = "Ç¨";  break;
					case 0x43: moji = "ÇÆ";  break;
					case 0x44: moji = "Ç∞";  break;
					case 0x45: moji = "Ç≤";  break;
					case 0x46: moji = "Ç¥";  break;
					case 0x47: moji = "Ç∂";  break;
					case 0x48: moji = "Ç∏";  break;
					case 0x49: moji = "Ç∫";  break;
					case 0x4A: moji = "Çº";  break;
					case 0x4B: moji = "Çæ";  break;
					case 0x4C: moji = "Ç¿";  break;
					case 0x4D: moji = "Ç√";  break;
					case 0x4E: moji = "Ç≈";  break;
					case 0x4F: moji = "Ç«";  break;
					case 0x50: moji = "ÇŒ";  break;
					case 0x51: moji = "Ç—";  break;
					case 0x52: moji = "Ç‘";  break;
					case 0x53: moji = "Ç◊";  break;
					case 0x54: moji = "Ç⁄";  break;
					case 0x55: moji = "Çœ";  break;
					case 0x56: moji = "Ç“";  break;
					case 0x57: moji = "Ç’";  break;
					case 0x58: moji = "Çÿ";  break;
					case 0x59: moji = "Ç€";  break;
					case 0x5A: moji = "Ç`";  break;
					case 0x5B: moji = "Ça";  break;
					case 0x5C: moji = "Çb";  break;
					case 0x5D: moji = "Çc";  break;
					case 0x5E: moji = "Çd";  break;
					case 0x5F: moji = "Çe";  break;
					case 0x60: moji = "Çf";  break;
					case 0x61: moji = "Çg";  break;
					case 0x62: moji = "Çh";  break;
					case 0x63: moji = "Çi";  break;
					case 0x64: moji = "Çj";  break;
					case 0x65: moji = "Çk";  break;
					case 0x66: moji = "Çl";  break;
					case 0x67: moji = "Çm";  break;
					case 0x68: moji = "Çn";  break;
					case 0x69: moji = "Ço";  break;
					case 0x6A: moji = "Çp";  break;
					case 0x6B: moji = "Çq";  break;
					case 0x6C: moji = "Çr";  break;
					case 0x6D: moji = "Çs";  break;
					case 0x6E: moji = "Çt";  break;
					case 0x6F: moji = "Çu";  break;
					case 0x70: moji = "Çv";  break;
					case 0x71: moji = "Çw";  break;
					case 0x72: moji = "Çx";  break;
					case 0x73: moji = "Çy";  break;
					case 0x74: moji = "ÅE";  break;
					case 0x75: moji = "ÅF";  break;
					case 0x76: moji = "ÅG";  break;
					case 0x77: moji = "Å`";  break;
					case 0x78: moji = "Å^";  break;
					case 0x79: moji = "Åi";  break;
					case 0x7A: moji = "Åj";  break;
					case 0x7B: moji = "êπ";  break;
					case 0x7C: moji = "åï";  break;
					case 0x7D: moji = "ì`";  break;
					case 0x7E: moji = "ê‡";  break;
					case 0x7F: moji = "Å@";  break;
					case 0x80: moji = "ÇO";  break;
					case 0x81: moji = "ÇP";  break;
					case 0x82: moji = "ÇQ";  break;
					case 0x83: moji = "ÇR";  break;
					case 0x84: moji = "ÇS";  break;
					case 0x85: moji = "ÇT";  break;
					case 0x86: moji = "ÇU";  break;
					case 0x87: moji = "ÇV";  break;
					case 0x88: moji = "ÇW";  break;
					case 0x89: moji = "ÇX";  break;
					case 0x8A: moji = "ÉA";  break;
					case 0x8B: moji = "ÉC";  break;
					case 0x8C: moji = "ÉE";  break;
					case 0x8D: moji = "ÉG";  break;
					case 0x8E: moji = "ÉI";  break;
					case 0x8F: moji = "ÉJ";  break;
					case 0x90: moji = "ÉL";  break;
					case 0x91: moji = "ÉN";  break;
					case 0x92: moji = "ÉP";  break;
					case 0x93: moji = "ÉR";  break;
					case 0x94: moji = "ÉT";  break;
					case 0x95: moji = "ÉV";  break;
					case 0x96: moji = "ÉX";  break;
					case 0x97: moji = "ÉZ";  break;
					case 0x98: moji = "É\\";  break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x99: moji = "É^";  break;
					case 0x9A: moji = "É`";  break;
					case 0x9B: moji = "Éc";  break;
					case 0x9C: moji = "Ée";  break;
					case 0x9D: moji = "Ég";  break;
					case 0x9E: moji = "Éi";  break;
					case 0x9F: moji = "Éj";  break;
					case 0xA0: moji = "Ék";  break;
					case 0xA1: moji = "Él";  break;
					case 0xA2: moji = "Ém";  break;
					case 0xA3: moji = "Én";  break;
					case 0xA4: moji = "Éq";  break;
					case 0xA5: moji = "Ét";  break;
					case 0xA6: moji = "Éw";  break;
					case 0xA7: moji = "Éz";  break;
					case 0xA8: moji = "É}";  break;
					case 0xA9: moji = "É~";  break;
					case 0xAA: moji = "ÉÄ";  break;
					case 0xAB: moji = "ÉÅ";  break;
					case 0xAC: moji = "ÉÇ";  break;
					case 0xAD: moji = "ÉÑ";  break;
					case 0xAE: moji = "ÉÜ";  break;
					case 0xAF: moji = "Éà";  break;
					case 0xB0: moji = "Éâ";  break;
					case 0xB1: moji = "Éä";  break;
					case 0xB2: moji = "Éã";  break;
					case 0xB3: moji = "Éå";  break;
					case 0xB4: moji = "Éç";  break;
					case 0xB5: moji = "Éè";  break;
					case 0xB6: moji = "Éì";  break;
					case 0xB7: moji = "Éí";  break;
					case 0xB8: moji = "Éb";  break;
					case 0xB9: moji = "ÉÉ";  break;
					case 0xBA: moji = "ÉÖ";  break;
					case 0xBB: moji = "Éá";  break;
					case 0xBC: moji = "É@";  break;
					case 0xBD: moji = "ÉB";  break;
					case 0xBE: moji = "ÉD";  break;
					case 0xBF: moji = "ÉF";  break;
					case 0xC0: moji = "ÉH";  break;
					case 0xC1: moji = "ÉK";  break;
					case 0xC2: moji = "ÉM";  break;
					case 0xC3: moji = "ÉO";  break;
					case 0xC4: moji = "ÉQ";  break;
					case 0xC5: moji = "ÉS";  break;
					case 0xC6: moji = "ÉU";  break;
					case 0xC7: moji = "ÉW";  break;
					case 0xC8: moji = "ÉY";  break;
					case 0xC9: moji = "É[";  break;
					case 0xCA: moji = "É]";  break;
					case 0xCB: moji = "É_";  break;
					case 0xCC: moji = "Éa";  break;
					case 0xCD: moji = "Éd";  break;
					case 0xCE: moji = "Éf";  break;
					case 0xCF: moji = "Éh";  break;
					case 0xD0: moji = "Éo";  break;
					case 0xD1: moji = "Ér";  break;
					case 0xD2: moji = "Éu";  break;
					case 0xD3: moji = "Éx";  break;
					case 0xD4: moji = "É{";  break;
					case 0xD5: moji = "Ép";  break;
					case 0xD6: moji = "És";  break;
					case 0xD7: moji = "Év";  break;
					case 0xD8: moji = "Éy";  break;
					case 0xD9: moji = "É|";  break;
					case 0xDA: moji = "ÇÅ";  break;
					case 0xDB: moji = "ÇÇ";  break;
					case 0xDC: moji = "ÇÉ";  break;
					case 0xDD: moji = "ÇÑ";  break;
					case 0xDE: moji = "ÇÖ";  break;
					case 0xDF: moji = "ÇÜ";  break;
					case 0xE0: moji = "Çá";  break;
					case 0xE1: moji = "Çà";  break;
					case 0xE2: moji = "Çâ";  break;
					case 0xE3: moji = "Çä";  break;
					case 0xE4: moji = "Çã";  break;
					case 0xE5: moji = "Çå";  break;
					case 0xE6: moji = "Çç";  break;
					case 0xE7: moji = "Çé";  break;
					case 0xE8: moji = "Çè";  break;
					case 0xE9: moji = "Çê";  break;
					case 0xEA: moji = "Çë";  break;
					case 0xEB: moji = "Çí";  break;
					case 0xEC: moji = "Çì";  break;
					case 0xED: moji = "Çî";  break;
					case 0xEE: moji = "Çï";  break;
					case 0xEF: moji = "Çñ";  break;
					case 0xF0: moji = "Çó";  break;
					case 0xF1: moji = "Çò";  break;
					case 0xF2: moji = "Çô";  break;
					case 0xF3: moji = "Çö";  break;
					case 0xF4: moji = "Éî";  break;
					case 0xF5: moji = "ÅX";  break;
					case 0xF6: moji = "óé";  break;
					case 0xF7: moji = "à´";  break;
					case 0xF8: moji = "ì¸";  break;
					case 0xF9: moji = "éË";  break;
					case 0xFA: moji = "óp";  break;
					case 0xFB: moji = "êÊ";  break;
					case 0xFC: moji = "à¿";  break;
					case 0xFD: moji = "íN";  break;
					case 0xFE: moji = "âπ";  break;
					case 0xFF: moji = "Å§";  break;
				}
				gPrint(moji,pDUMP,col,row);
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
					case 0x0: moji = "ÇO";  break;
					case 0x1: moji = "ÇP";  break;
					case 0x2: moji = "ÇQ";  break;
					case 0x3: moji = "ÇR";  break;
					case 0x4: moji = "ÇS";  break;
					case 0x5: moji = "ÇT";  break;
					case 0x6: moji = "ÇU";  break;
					case 0x7: moji = "ÇV";  break;
					case 0x8: moji = "ÇW";  break;
					case 0x9: moji = "ÇX";  break;
					case 0x0A: moji = "ÉA";  break;
					case 0x0B: moji = "ÉC";  break;
					case 0x0C: moji = "ÉE";  break;
					case 0x0D: moji = "ÉG";  break;
					case 0x0E: moji = "ÉI";  break;
					case 0x0F: moji = "ÉJ";  break;
					case 0x10: moji = "ÉL";  break;
					case 0x11: moji = "ÉN";  break;
					case 0x12: moji = "ÉP";  break;
					case 0x13: moji = "ÉR";  break;
					case 0x14: moji = "ÉT";  break;
					case 0x15: moji = "ÉV";  break;
					case 0x16: moji = "ÉX";  break;
					case 0x17: moji = "ÉZ";  break;
					case 0x18: moji = "É\\";  break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x19: moji = "É^";  break;
					case 0x1A: moji = "É`";  break;
					case 0x1B: moji = "Éc";  break;
					case 0x1C: moji = "Ée";  break;
					case 0x1D: moji = "Ég";  break;
					case 0x1E: moji = "Éi";  break;
					case 0x1F: moji = "Éj";  break;
					case 0x20: moji = "Ék";  break;
					case 0x21: moji = "Él";  break;
					case 0x22: moji = "Ém";  break;
					case 0x23: moji = "Én";  break;
					case 0x24: moji = "Éq";  break;
					case 0x25: moji = "Ét";  break;
					case 0x26: moji = "Éw";  break;
					case 0x27: moji = "Éz";  break;
					case 0x28: moji = "É}";  break;
					case 0x29: moji = "É~";  break;
					case 0x2A: moji = "ÉÄ";  break;
					case 0x2B: moji = "ÉÅ";  break;
					case 0x2C: moji = "ÉÇ";  break;
					case 0x2D: moji = "ÉÑ";  break;
					case 0x2E: moji = "ÉÜ";  break;
					case 0x2F: moji = "Éà";  break;
					case 0x30: moji = "Éâ";  break;
					case 0x31: moji = "Éä";  break;
					case 0x32: moji = "Éã";  break;
					case 0x33: moji = "Éå";  break;
					case 0x34: moji = "Éç";  break;
					case 0x35: moji = "Éè";  break;
					case 0x36: moji = "Éì";  break;
					case 0x37: moji = "Éí";  break;
					case 0x38: moji = "Éb";  break;
					case 0x39: moji = "ÉÉ";  break;
					case 0x3A: moji = "ÉÖ";  break;
					case 0x3B: moji = "Éá";  break;
					case 0x3C: moji = "É@";  break;
					case 0x3D: moji = "ÉB";  break;
					case 0x3E: moji = "ÉD";  break;
					case 0x3F: moji = "ÉF";  break;
					case 0x40: moji = "ÉH";  break;
					case 0x41: moji = "ÉK";  break;
					case 0x42: moji = "ÉM";  break;
					case 0x43: moji = "ÉO";  break;
					case 0x44: moji = "ÉQ";  break;
					case 0x45: moji = "ÉS";  break;
					case 0x46: moji = "ÉU";  break;
					case 0x47: moji = "ÉW";  break;
					case 0x48: moji = "ÉY";  break;
					case 0x49: moji = "É[";  break;
					case 0x4A: moji = "É]";  break;
					case 0x4B: moji = "É_";  break;
					case 0x4C: moji = "Éa";  break;
					case 0x4D: moji = "Éd";  break;
					case 0x4E: moji = "Éf";  break;
					case 0x4F: moji = "Éh";  break;
					case 0x50: moji = "Éo";  break;
					case 0x51: moji = "Ér";  break;
					case 0x52: moji = "Éu";  break;
					case 0x53: moji = "Éx";  break;
					case 0x54: moji = "É{";  break;
					case 0x55: moji = "Ép";  break;
					case 0x56: moji = "És";  break;
					case 0x57: moji = "Év";  break;
					case 0x58: moji = "Éy";  break;
					case 0x59: moji = "É|";  break;
					case 0x5A: moji = "ÇÅ";  break;
					case 0x5B: moji = "ÇÇ";  break;
					case 0x5C: moji = "ÇÉ";  break;
					case 0x5D: moji = "ÇÑ";  break;
					case 0x5E: moji = "ÇÖ";  break;
					case 0x5F: moji = "ÇÜ";  break;
					case 0x60: moji = "Çá";  break;
					case 0x61: moji = "Çà";  break;
					case 0x62: moji = "Çâ";  break;
					case 0x63: moji = "Çä";  break;
					case 0x64: moji = "Çã";  break;
					case 0x65: moji = "Çå";  break;
					case 0x66: moji = "Çç";  break;
					case 0x67: moji = "Çé";  break;
					case 0x68: moji = "Çè";  break;
					case 0x69: moji = "Çê";  break;
					case 0x6A: moji = "Çë";  break;
					case 0x6B: moji = "Çí";  break;
					case 0x6C: moji = "Çì";  break;
					case 0x6D: moji = "Çî";  break;
					case 0x6E: moji = "Çï";  break;
					case 0x6F: moji = "Çñ";  break;
					case 0x70: moji = "Çó";  break;
					case 0x71: moji = "Çò";  break;
					case 0x72: moji = "Çô";  break;
					case 0x73: moji = "Çö";  break;
					case 0x74: moji = "Éî";  break;
					case 0x75: moji = "ÅX";  break;
					case 0x76: moji = "óé";  break;
					case 0x77: moji = "à´";  break;
					case 0x78: moji = "ì¸";  break;
					case 0x79: moji = "éË";  break;
					case 0x7A: moji = "óp";  break;
					case 0x7B: moji = "êÊ";  break;
					case 0x7C: moji = "à¿";  break;
					case 0x7D: moji = "íN";  break;
					case 0x7E: moji = "âπ";  break;
					case 0x7F: moji = "Å§";  break;
					case 0x80: moji = "éÂ";  break;
					case 0x81: moji = "êl";  break;
					case 0x82: moji = "í¨";  break;
					case 0x83: moji = "êX";  break;
					case 0x84: moji = "óà";  break;
					case 0x85: moji = "ä‘";  break;
					case 0x86: moji = "év";  break;
					case 0x87: moji = "çá";  break;
					case 0x88: moji = "åı";  break;
					case 0x89: moji = "è¡";  break;
					case 0x8A: moji = "å`";  break;
					case 0x8B: moji = "âÒ";  break;
					case 0x8C: moji = "ç¢";  break;
					case 0x8D: moji = "í∑";  break;
					case 0x8E: moji = "òV";  break;
					case 0x8F: moji = "ìå";  break;
					case 0x90: moji = "êº";  break;
					case 0x91: moji = "ó";  break;
					case 0x92: moji = "éj";  break;
					case 0x93: moji = "âÔ";  break;
					case 0x94: moji = "ä÷";  break;
					case 0x95: moji = "åW";  break;
					case 0x96: moji = "ê^";  break;
					case 0x97: moji = "ñk";  break;
					case 0x98: moji = "ìÏ";  break;
					case 0x99: moji = "ëÂ";  break;
					case 0x9A: moji = "ín";  break;
					case 0x9B: moji = "êŒ";  break;
					case 0x9C: moji = "ñ⁄";  break;
					case 0x9D: moji = "âΩ";  break;
					case 0x9E: moji = "ó—";  break;
					case 0x9F: moji = "í ";  break;
					case 0xA0: moji = "íj";  break;
					case 0xA1: moji = "å„";  break;
					case 0xA2: moji = "ìX";  break;
					case 0xA3: moji = "ñ∫";  break;
					case 0xA4: moji = "èó";  break;
					case 0xA5: moji = "ãü";  break;
					case 0xA6: moji = "íÈ";  break;
					case 0xA7: moji = "çë";  break;
					case 0xA8: moji = "éÑ";  break;
					case 0xA9: moji = "ãq";  break;
					case 0xAA: moji = "âª";  break;
					case 0xAB: moji = "çs";  break;
					case 0xAC: moji = "ï˚";  break;
					case 0xAD: moji = "ñæ";  break;
					case 0xAE: moji = "çc";  break;
					case 0xAF: moji = "óv";  break;
					case 0xB0: moji = "ç«";  break;
					case 0xB1: moji = "âJ";  break;
					case 0xB2: moji = "ñÿ";  break;
					case 0xB3: moji = "ñ≈";  break;
					case 0xB4: moji = "åN";  break;
					case 0xB5: moji = "Åì";  break;
					case 0xB6: moji = "ê∂";  break;
					case 0xB7: moji = "åå";  break;
					case 0xB8: moji = "ç≤";  break;
					case 0xB9: moji = "éÁ";  break;
					case 0xBA: moji = "êÌ";  break;
					case 0xBB: moji = "ëà";  break;
					case 0xBC: moji = "ãR";  break;
					case 0xBD: moji = "ém";  break;
					case 0xBE: moji = "ìG";  break;
					case 0xBF: moji = "åä";  break;
					case 0xC0: moji = "å¸";  break;
					case 0xC1: moji = "à¯";  break;
					case 0xC2: moji = "é◊";  break;
					case 0xC3: moji = "ìÆ";  break;
					case 0xC4: moji = "ê¨";  break;
					case 0xC5: moji = "å˜";  break;
					case 0xC6: moji = "ïv";  break;
					case 0xC7: moji = "ï∂";  break;
					case 0xC8: moji = "é·";  break;
					case 0xC9: moji = "ó¨";  break;
					case 0xCA: moji = "é∏";  break;
					case 0xCB: moji = "êÖ";  break;
					case 0xCC: moji = "äO";  break;
					case 0xCD: moji = "é©";  break;
					case 0xCE: moji = "ï™";  break;
					case 0xCF: moji = "êg";  break;
					case 0xD0: moji = "ñ{";  break;
					case 0xD1: moji = "é„";  break;
					case 0xD2: moji = "é∏";  break;
					case 0xD3: moji = "äB";  break;
					case 0xD4: moji = "å´";  break;
					case 0xD5: moji = "é“";  break;
					case 0xD6: moji = "äC";  break;
					case 0xD7: moji = "ìá";  break;
					case 0xD8: moji = "òb";  break;
					case 0xD9: moji = "äE";  break;
					case 0xDA: moji = "ëS";  break;
					case 0xDB: moji = "éÄ";  break;
					case 0xDC: moji = "çD";  break;
					case 0xDD: moji = "ç–";  break;
					case 0xDE: moji = "å√";  break;
					case 0xDF: moji = "ë„";  break;
					case 0xE0: moji = "ä‚";  break;
					case 0xE1: moji = "ïÛ";  break;
					case 0xE2: moji = "ïÉ";  break;
					case 0xE3: moji = "îN";  break;
					case 0xE4: moji = "ìa";  break;
					case 0xE5: moji = "ëê";  break;
					case 0xE6: moji = "çÇ";  break;
					case 0xE7: moji = "ã≠";  break;
					case 0xE8: moji = "êÿ";  break;
					case 0xE9: moji = "íá";  break;
					case 0xEA: moji = "ån";  break;
					case 0xEB: moji = "îí";  break;
					case 0xEC: moji = "ì≈";  break;
					case 0xED: moji = "é_";  break;
					case 0xEE: moji = "ï®";  break;
					case 0xEF: moji = "ïú";  break;
					case 0xF0: moji = "ì_";  break;
					case 0xF1: moji = "ñ∞";  break;
					case 0xF2: moji = "âF";  break;
					case 0xF3: moji = "íà";  break;
					case 0xF4: moji = "ó§";  break;
					case 0xF5: moji = "è≠";  break;
					case 0xF6: moji = "éR";  break;
					case 0xF7: moji = "éw";  break;
					case 0xF8: moji = "ëÍ";  break;
					case 0xF9: moji = "éñ";  break;
					case 0xFA: moji = "ìñ";  break;
					case 0xFB: moji = "MP";  break;
					case 0xFC: moji = "Å™";  break;
					case 0xFD: moji = "Å´";  break;
					case 0xFE: moji = "Å©";  break;
					case 0xFF: moji = "Å®";  break;
				}
				gPrint(moji,pDUMP,col,row);
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
					case 0x0: moji = "éÂ";  break;
					case 0x1: moji = "êl";  break;
					case 0x2: moji = "í¨";  break;
					case 0x3: moji = "êX";  break;
					case 0x4: moji = "óà";  break;
					case 0x5: moji = "ä‘";  break;
					case 0x6: moji = "év";  break;
					case 0x7: moji = "çá";  break;
					case 0x8: moji = "åı";  break;
					case 0x9: moji = "è¡";  break;
					case 0x0A: moji = "å`";  break;
					case 0x0B: moji = "âÒ";  break;
					case 0x0C: moji = "ç¢";  break;
					case 0x0D: moji = "í∑";  break;
					case 0x0E: moji = "òV";  break;
					case 0x0F: moji = "ìå";  break;
					case 0x10: moji = "êº";  break;
					case 0x11: moji = "ó";  break;
					case 0x12: moji = "éj";  break;
					case 0x13: moji = "âÔ";  break;
					case 0x14: moji = "ä÷";  break;
					case 0x15: moji = "åW";  break;
					case 0x16: moji = "ê^";  break;
					case 0x17: moji = "ñk";  break;
					case 0x18: moji = "ìÏ";  break;
					case 0x19: moji = "ëÂ";  break;
					case 0x1A: moji = "ín";  break;
					case 0x1B: moji = "êŒ";  break;
					case 0x1C: moji = "ñ⁄";  break;
					case 0x1D: moji = "âΩ";  break;
					case 0x1E: moji = "ó—";  break;
					case 0x1F: moji = "í ";  break;
					case 0x20: moji = "íj";  break;
					case 0x21: moji = "å„";  break;
					case 0x22: moji = "ìX";  break;
					case 0x23: moji = "ñ∫";  break;
					case 0x24: moji = "èó";  break;
					case 0x25: moji = "ãü";  break;
					case 0x26: moji = "íÈ";  break;
					case 0x27: moji = "çë";  break;
					case 0x28: moji = "éÑ";  break;
					case 0x29: moji = "ãq";  break;
					case 0x2A: moji = "âª";  break;
					case 0x2B: moji = "çs";  break;
					case 0x2C: moji = "ï˚";  break;
					case 0x2D: moji = "ñæ";  break;
					case 0x2E: moji = "çc";  break;
					case 0x2F: moji = "óv";  break;
					case 0x30: moji = "ç«";  break;
					case 0x31: moji = "âJ";  break;
					case 0x32: moji = "ñÿ";  break;
					case 0x33: moji = "ñ≈";  break;
					case 0x34: moji = "åN";  break;
					case 0x35: moji = "Åì";  break;
					case 0x36: moji = "ê∂";  break;
					case 0x37: moji = "åå";  break;
					case 0x38: moji = "ç≤";  break;
					case 0x39: moji = "éÁ";  break;
					case 0x3A: moji = "êÌ";  break;
					case 0x3B: moji = "ëà";  break;
					case 0x3C: moji = "ãR";  break;
					case 0x3D: moji = "ém";  break;
					case 0x3E: moji = "ìG";  break;
					case 0x3F: moji = "åä";  break;
					case 0x40: moji = "å¸";  break;
					case 0x41: moji = "à¯";  break;
					case 0x42: moji = "é◊";  break;
					case 0x43: moji = "ìÆ";  break;
					case 0x44: moji = "ê¨";  break;
					case 0x45: moji = "å˜";  break;
					case 0x46: moji = "ïv";  break;
					case 0x47: moji = "ï∂";  break;
					case 0x48: moji = "é·";  break;
					case 0x49: moji = "ó¨";  break;
					case 0x4A: moji = "é∏";  break;
					case 0x4B: moji = "êÖ";  break;
					case 0x4C: moji = "äO";  break;
					case 0x4D: moji = "é©";  break;
					case 0x4E: moji = "ï™";  break;
					case 0x4F: moji = "êg";  break;
					case 0x50: moji = "ñ{";  break;
					case 0x51: moji = "é„";  break;
					case 0x52: moji = "é∏";  break;
					case 0x53: moji = "äB";  break;
					case 0x54: moji = "å´";  break;
					case 0x55: moji = "é“";  break;
					case 0x56: moji = "äC";  break;
					case 0x57: moji = "ìá";  break;
					case 0x58: moji = "òb";  break;
					case 0x59: moji = "äE";  break;
					case 0x5A: moji = "ëS";  break;
					case 0x5B: moji = "éÄ";  break;
					case 0x5C: moji = "çD";  break;
					case 0x5D: moji = "ç–";  break;
					case 0x5E: moji = "å√";  break;
					case 0x5F: moji = "ë„";  break;
					case 0x60: moji = "ä‚";  break;
					case 0x61: moji = "ïÛ";  break;
					case 0x62: moji = "ïÉ";  break;
					case 0x63: moji = "îN";  break;
					case 0x64: moji = "ìa";  break;
					case 0x65: moji = "ëê";  break;
					case 0x66: moji = "çÇ";  break;
					case 0x67: moji = "ã≠";  break;
					case 0x68: moji = "êÿ";  break;
					case 0x69: moji = "íá";  break;
					case 0x6A: moji = "ån";  break;
					case 0x6B: moji = "îí";  break;
					case 0x6C: moji = "ì≈";  break;
					case 0x6D: moji = "é_";  break;
					case 0x6E: moji = "ï®";  break;
					case 0x6F: moji = "ïú";  break;
					case 0x70: moji = "ì_";  break;
					case 0x71: moji = "ñ∞";  break;
					case 0x72: moji = "âF";  break;
					case 0x73: moji = "íà";  break;
					case 0x74: moji = "ó§";  break;
					case 0x75: moji = "è≠";  break;
					case 0x76: moji = "éR";  break;
					case 0x77: moji = "éw";  break;
					case 0x78: moji = "ëÍ";  break;
					case 0x79: moji = "éñ";  break;
					case 0x7A: moji = "ìñ";  break;
					case 0x7B: moji = "MP";  break;
					case 0x7C: moji = "Å™";  break;
					case 0x7D: moji = "Å´";  break;
					case 0x7E: moji = "Å©";  break;
					case 0x7F: moji = "Å®";  break;
					case 0x80: moji = "âü";  break;
					case 0x81: moji = "à»";  break;
					case 0x82: moji = "èÈ";  break;
					case 0x83: moji = "éö";  break;
					case 0x84: moji = "è\\";  break;//more wierdness
					case 0x85: moji = "åà";  break;
					case 0x86: moji = "ëI";  break;
					case 0x87: moji = "íË";  break;
					case 0x88: moji = "éû";  break;
					case 0x89: moji = "ç∂";  break;
					case 0x8A: moji = "âE";  break;
					case 0x8B: moji = "êi";  break;
					case 0x8C: moji = "éü";  break;
					case 0x8D: moji = "ñﬂ";  break;
					case 0x8E: moji = "ñÇ";  break;
					case 0x8F: moji = "å©";  break;
					case 0x90: moji = "ñ@";  break;
					case 0x91: moji = "óÕ";  break;
					case 0x92: moji = "êl";  break;
					case 0x93: moji = "äÌ";  break;
					case 0x94: moji = "î";  break;
					case 0x95: moji = "ïê";  break;
					case 0x96: moji = "ïœ";  break;
					case 0x97: moji = "äe";  break;
					case 0x98: moji = "çU";  break;
					case 0x99: moji = "én";  break;
					case 0x9A: moji = "åÇ";  break;
					case 0x9B: moji = "è¡";  break;
					case 0x9C: moji = "âÒ";  break;
					case 0x9D: moji = "èI";  break;
					case 0x9E: moji = "ñh";  break;
					case 0x9F: moji = "å‰";  break;
					case 0xA0: moji = "âÆ";  break;
					case 0xA1: moji = "àÍ";  break;
					case 0xA2: moji = "åª";  break;
					case 0xA3: moji = "ç≈";  break;
					case 0xA4: moji = "èâ";  break;
					case 0xA5: moji = "ìØ";  break;
					case 0xA6: moji = "è„";  break;
					case 0xA7: moji = "â∫";  break;
					case 0xA8: moji = "ãZ";  break;
					case 0xA9: moji = "ñ ";  break;
					case 0xAA: moji = "ã§";  break;
					case 0xAB: moji = "ó∑";  break;
					case 0xAC: moji = "ñ¬";  break;
					case 0xAD: moji = "ïΩ";  break;
					case 0xAE: moji = "ê∏";  break;
					case 0xAF: moji = "òa";  break;
					case 0xB0: moji = "ím";  break;
					case 0xB1: moji = "ëÃ";  break;
					case 0xB2: moji = "ïX";  break;
					case 0xB3: moji = "ëD";  break;
					case 0xB4: moji = "íÜ";  break;
					case 0xB5: moji = "ê_";  break;
					case 0xB6: moji = "ñΩ";  break;
					case 0xB7: moji = "ëO";  break;
					case 0xB8: moji = "è¨";  break;
					case 0xB9: moji = "ëΩ";  break;
					case 0xBA: moji = "ê¢";  break;
					case 0xBB: moji = "çÏ";  break;
					case 0xBC: moji = "ë±";  break;
					case 0xBD: moji = "äE";  break;
					case 0xBE: moji = "èb";  break;
					case 0xBF: moji = "ã‡";  break;
					case 0xC0: moji = "ã@";  break;
					case 0xC1: moji = "ê⁄";  break;
					case 0xC2: moji = "ãﬂ";  break;
					case 0xC3: moji = "ë“";  break;
					case 0xC4: moji = "ñº";  break;
					case 0xC5: moji = "óE";  break;
					case 0xC6: moji = "ãC";  break;
					case 0xC7: moji = "à§";  break;
					case 0xC8: moji = "åb";  break;
					case 0xC9: moji = "êS";  break;
					case 0xCA: moji = "çª";  break;
					case 0xCB: moji = "îô";  break;
					case 0xCC: moji = "íÍ";  break;
					case 0xCD: moji = "êÃ";  break;
					case 0xCE: moji = "êU";  break;
					case 0xCF: moji = "ãN";  break;
					case 0xD0: moji = "ìÅ";  break;
					case 0xD1: moji = "âŒ";  break;
					case 0xD2: moji = "âä";  break;
					case 0xD3: moji = "ó≥";  break;
					case 0xD4: moji = "ìV";  break;
					case 0xD5: moji = "â©";  break;
					case 0xD6: moji = "ãπ";  break;
					case 0xD7: moji = "ég";  break;
					case 0xD8: moji = "à≈";  break;
					case 0xD9: moji = "ìπ";  break;
					case 0xDA: moji = "íÖ";  break;
					case 0xDB: moji = "äv";  break;
					case 0xDC: moji = "â§";  break;
					case 0xDD: moji = "ä•";  break;
					case 0xDE: moji = "óÏ";  break;
					case 0xDF: moji = "âH";  break;
					case 0xE0: moji = "ïÍ";  break;
					case 0xE1: moji = "î≤";  break;
					case 0xE2: moji = "èo";  break;
					case 0xE3: moji = "ë∫";  break;
					case 0xE4: moji = "éq";  break;
					case 0xE5: moji = "ñC";  break;
					case 0xE6: moji = "îö";  break;
					case 0xE7: moji = "íe";  break;
					case 0xE8: moji = "êj";  break;
					case 0xE9: moji = "ît";  break;
					case 0xEA: moji = "ñÚ";  break;
					case 0xEB: moji = "ïó";  break;
					case 0xEC: moji = "ëæ";  break;
					case 0xED: moji = "å€";  break;
					case 0xEE: moji = "éÌ";  break;
					case 0xEF: moji = "î†";  break;
					case 0xF0: moji = "ñ°";  break;
					case 0xF1: moji = "ó†";  break;
					case 0xF2: moji = "ä€";  break;
					case 0xF3: moji = "åé";  break;
					case 0xF4: moji = "ó÷";  break;
					case 0xF5: moji = "ã‚";  break;
					case 0xF6: moji = "ñÓ";  break;
					case 0xF7: moji = "ã|";  break;
					case 0xF8: moji = "ìS";  break;
					case 0xF9: moji = "î\\";  break;//more wierdness
					case 0xFA: moji = "ãê";  break;
					case 0xFB: moji = "ód";  break;
					case 0xFC: moji = "èC";  break;
					case 0xFD: moji = "ïï";  break;
					case 0xFE: moji = "àÛ";  break;
					case 0xFF: moji = "ï∑";  break;
				}
				gPrint(moji,pDUMP,col,row);
			}
		}
		else
		{
			//print any unknown bytes
			fprintf(pDUMP,DEBUG ? "{%0.2X}" : "",cur);
		}
	}
	
	//denote end of file
	fprintf(pDUMP,"<END>");
	
	//close files
	fclose(pROM);
	fclose(pDUMP);
	
	return 0;
}
