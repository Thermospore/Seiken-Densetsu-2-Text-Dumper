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
				fprintf(pDUMP,"<ÉâÉìÉfÉB>");
				col += 4;
			} 
			else if(cur == 0x01)
			{
				fprintf(pDUMP,"<ÉvÉäÉÄ>");
				col += 3;
			}
			else if(cur == 0x02)
			{
				fprintf(pDUMP,"<É|É|ÉC>");
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
			    case 0x80: fprintf(pDUMP,"ÅA"); break;
				case 0x81: fprintf(pDUMP,"ÅB"); break;
				case 0x82: fprintf(pDUMP,"ÅI"); break;
				case 0x83: fprintf(pDUMP,"ÅH"); break;
				case 0x84: fprintf(pDUMP,"Å["); break;
				case 0x85: fprintf(pDUMP,"Åd"); break;
				case 0x86: fprintf(pDUMP,"Åu"); break;
				case 0x87: fprintf(pDUMP,"Åv"); break;
				case 0x88: fprintf(pDUMP,"Åw"); break;
				case 0x89: fprintf(pDUMP,"Åx"); break;
				case 0x8A: fprintf(pDUMP,"Ç†"); break;
				case 0x8B: fprintf(pDUMP,"Ç¢"); break;
				case 0x8C: fprintf(pDUMP,"Ç§"); break;
				case 0x8D: fprintf(pDUMP,"Ç¶"); break;
				case 0x8E: fprintf(pDUMP,"Ç®"); break;
				case 0x8F: fprintf(pDUMP,"Ç©"); break;
				case 0x90: fprintf(pDUMP,"Ç´"); break;
				case 0x91: fprintf(pDUMP,"Ç≠"); break;
				case 0x92: fprintf(pDUMP,"ÇØ"); break;
				case 0x93: fprintf(pDUMP,"Ç±"); break;
				case 0x94: fprintf(pDUMP,"Ç≥"); break;
				case 0x95: fprintf(pDUMP,"Çµ"); break;
				case 0x96: fprintf(pDUMP,"Ç∑"); break;
				case 0x97: fprintf(pDUMP,"Çπ"); break;
				case 0x98: fprintf(pDUMP,"Çª"); break;
				case 0x99: fprintf(pDUMP,"ÇΩ"); break;
				case 0x9A: fprintf(pDUMP,"Çø"); break;
				case 0x9B: fprintf(pDUMP,"Ç¬"); break;
				case 0x9C: fprintf(pDUMP,"Çƒ"); break;
				case 0x9D: fprintf(pDUMP,"Ç∆"); break;
				case 0x9E: fprintf(pDUMP,"Ç»"); break;
				case 0x9F: fprintf(pDUMP,"Ç…"); break;
				case 0xA0: fprintf(pDUMP,"Ç "); break;
				case 0xA1: fprintf(pDUMP,"ÇÀ"); break;
				case 0xA2: fprintf(pDUMP,"ÇÃ"); break;
				case 0xA3: fprintf(pDUMP,"ÇÕ"); break;
				case 0xA4: fprintf(pDUMP,"Ç–"); break;
				case 0xA5: fprintf(pDUMP,"Ç”"); break;
				case 0xA6: fprintf(pDUMP,"Ç÷"); break;
				case 0xA7: fprintf(pDUMP,"ÇŸ"); break;
				case 0xA8: fprintf(pDUMP,"Ç‹"); break;
				case 0xA9: fprintf(pDUMP,"Ç›"); break;
				case 0xAA: fprintf(pDUMP,"Çﬁ"); break;
				case 0xAB: fprintf(pDUMP,"Çﬂ"); break;
				case 0xAC: fprintf(pDUMP,"Ç‡"); break;
				case 0xAD: fprintf(pDUMP,"Ç‚"); break;
				case 0xAE: fprintf(pDUMP,"Ç‰"); break;
				case 0xAF: fprintf(pDUMP,"ÇÊ"); break;
				case 0xB0: fprintf(pDUMP,"ÇÁ"); break;
				case 0xB1: fprintf(pDUMP,"ÇË"); break;
				case 0xB2: fprintf(pDUMP,"ÇÈ"); break;
				case 0xB3: fprintf(pDUMP,"ÇÍ"); break;
				case 0xB4: fprintf(pDUMP,"ÇÎ"); break;
				case 0xB5: fprintf(pDUMP,"ÇÌ"); break;
				case 0xB6: fprintf(pDUMP,"ÇÒ"); break;
				case 0xB7: fprintf(pDUMP,"Ç"); break;
				case 0xB8: fprintf(pDUMP,"Ç¡"); break;
				case 0xB9: fprintf(pDUMP,"Ç·"); break;
				case 0xBA: fprintf(pDUMP,"Ç„"); break;
				case 0xBB: fprintf(pDUMP,"ÇÂ"); break;
				case 0xBC: fprintf(pDUMP,"Çü"); break;
				case 0xBD: fprintf(pDUMP,"Ç°"); break;
				case 0xBE: fprintf(pDUMP,"Ç£"); break;
				case 0xBF: fprintf(pDUMP,"Ç•"); break;
				case 0xC0: fprintf(pDUMP,"Çß"); break;
				case 0xC1: fprintf(pDUMP,"Ç™"); break;
				case 0xC2: fprintf(pDUMP,"Ç¨"); break;
				case 0xC3: fprintf(pDUMP,"ÇÆ"); break;
				case 0xC4: fprintf(pDUMP,"Ç∞"); break;
				case 0xC5: fprintf(pDUMP,"Ç≤"); break;
				case 0xC6: fprintf(pDUMP,"Ç¥"); break;
				case 0xC7: fprintf(pDUMP,"Ç∂"); break;
				case 0xC8: fprintf(pDUMP,"Ç∏"); break;
				case 0xC9: fprintf(pDUMP,"Ç∫"); break;
				case 0xCA: fprintf(pDUMP,"Çº"); break;
				case 0xCB: fprintf(pDUMP,"Çæ"); break;
				case 0xCC: fprintf(pDUMP,"Ç¿"); break;
				case 0xCD: fprintf(pDUMP,"Ç√"); break;
				case 0xCE: fprintf(pDUMP,"Ç≈"); break;
				case 0xCF: fprintf(pDUMP,"Ç«"); break;
				case 0xD0: fprintf(pDUMP,"ÇŒ"); break;
				case 0xD1: fprintf(pDUMP,"Ç—"); break;
				case 0xD2: fprintf(pDUMP,"Ç‘"); break;
				case 0xD3: fprintf(pDUMP,"Ç◊"); break;
				case 0xD4: fprintf(pDUMP,"Ç⁄"); break;
				case 0xD5: fprintf(pDUMP,"Çœ"); break;
				case 0xD6: fprintf(pDUMP,"Ç“"); break;
				case 0xD7: fprintf(pDUMP,"Ç’"); break;
				case 0xD8: fprintf(pDUMP,"Çÿ"); break;
				case 0xD9: fprintf(pDUMP,"Ç€"); break;
				case 0xDA: fprintf(pDUMP,"Ç`"); break;
				case 0xDB: fprintf(pDUMP,"Ça"); break;
				case 0xDC: fprintf(pDUMP,"Çb"); break;
				case 0xDD: fprintf(pDUMP,"Çc"); break;
				case 0xDE: fprintf(pDUMP,"Çd"); break;
				case 0xDF: fprintf(pDUMP,"Çe"); break;
				case 0xE0: fprintf(pDUMP,"Çf"); break;
				case 0xE1: fprintf(pDUMP,"Çg"); break;
				case 0xE2: fprintf(pDUMP,"Çh"); break;
				case 0xE3: fprintf(pDUMP,"Çi"); break;
				case 0xE4: fprintf(pDUMP,"Çj"); break;
				case 0xE5: fprintf(pDUMP,"Çk"); break;
				case 0xE6: fprintf(pDUMP,"Çl"); break;
				case 0xE7: fprintf(pDUMP,"Çm"); break;
				case 0xE8: fprintf(pDUMP,"Çn"); break;
				case 0xE9: fprintf(pDUMP,"Ço"); break;
				case 0xEA: fprintf(pDUMP,"Çp"); break;
				case 0xEB: fprintf(pDUMP,"Çq"); break;
				case 0xEC: fprintf(pDUMP,"Çr"); break;
				case 0xED: fprintf(pDUMP,"Çs"); break;
				case 0xEE: fprintf(pDUMP,"Çt"); break;
				case 0xEF: fprintf(pDUMP,"Çu"); break;
				case 0xF0: fprintf(pDUMP,"Çv"); break;
				case 0xF1: fprintf(pDUMP,"Çw"); break;
				case 0xF2: fprintf(pDUMP,"Çx"); break;
				case 0xF3: fprintf(pDUMP,"Çy"); break;
				case 0xF4: fprintf(pDUMP,"ÅE"); break;
				case 0xF5: fprintf(pDUMP,"ÅF"); break;
				case 0xF6: fprintf(pDUMP,"ÅG"); break;
				case 0xF7: fprintf(pDUMP,"Å`"); break;
				case 0xF8: fprintf(pDUMP,"Å^"); break;
				case 0xF9: fprintf(pDUMP,"Åi"); break;
				case 0xFA: fprintf(pDUMP,"Åj"); break;
				case 0xFB: fprintf(pDUMP,"êπ"); break;
				case 0xFC: fprintf(pDUMP,"åï"); break;
				case 0xFD: fprintf(pDUMP,"ì`"); break;
				case 0xFE: fprintf(pDUMP,"ê‡"); break;
				case 0xFF: fprintf(pDUMP,"Å@"); break;
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
					case 0x0: fprintf(pDUMP,"ÅA"); break;
					case 0x1: fprintf(pDUMP,"ÅB"); break;
					case 0x2: fprintf(pDUMP,"ÅI"); break;
					case 0x3: fprintf(pDUMP,"ÅH"); break;
					case 0x4: fprintf(pDUMP,"Å["); break;
					case 0x5: fprintf(pDUMP,"Åd"); break;
					case 0x6: fprintf(pDUMP,"Åu"); break;
					case 0x7: fprintf(pDUMP,"Åv"); break;
					case 0x8: fprintf(pDUMP,"Åw"); break;
					case 0x9: fprintf(pDUMP,"Åx"); break;
					case 0x0A: fprintf(pDUMP,"Ç†"); break;
					case 0x0B: fprintf(pDUMP,"Ç¢"); break;
					case 0x0C: fprintf(pDUMP,"Ç§"); break;
					case 0x0D: fprintf(pDUMP,"Ç¶"); break;
					case 0x0E: fprintf(pDUMP,"Ç®"); break;
					case 0x0F: fprintf(pDUMP,"Ç©"); break;
					case 0x10: fprintf(pDUMP,"Ç´"); break;
					case 0x11: fprintf(pDUMP,"Ç≠"); break;
					case 0x12: fprintf(pDUMP,"ÇØ"); break;
					case 0x13: fprintf(pDUMP,"Ç±"); break;
					case 0x14: fprintf(pDUMP,"Ç≥"); break;
					case 0x15: fprintf(pDUMP,"Çµ"); break;
					case 0x16: fprintf(pDUMP,"Ç∑"); break;
					case 0x17: fprintf(pDUMP,"Çπ"); break;
					case 0x18: fprintf(pDUMP,"Çª"); break;
					case 0x19: fprintf(pDUMP,"ÇΩ"); break;
					case 0x1A: fprintf(pDUMP,"Çø"); break;
					case 0x1B: fprintf(pDUMP,"Ç¬"); break;
					case 0x1C: fprintf(pDUMP,"Çƒ"); break;
					case 0x1D: fprintf(pDUMP,"Ç∆"); break;
					case 0x1E: fprintf(pDUMP,"Ç»"); break;
					case 0x1F: fprintf(pDUMP,"Ç…"); break;
					case 0x20: fprintf(pDUMP,"Ç "); break;
					case 0x21: fprintf(pDUMP,"ÇÀ"); break;
					case 0x22: fprintf(pDUMP,"ÇÃ"); break;
					case 0x23: fprintf(pDUMP,"ÇÕ"); break;
					case 0x24: fprintf(pDUMP,"Ç–"); break;
					case 0x25: fprintf(pDUMP,"Ç”"); break;
					case 0x26: fprintf(pDUMP,"Ç÷"); break;
					case 0x27: fprintf(pDUMP,"ÇŸ"); break;
					case 0x28: fprintf(pDUMP,"Ç‹"); break;
					case 0x29: fprintf(pDUMP,"Ç›"); break;
					case 0x2A: fprintf(pDUMP,"Çﬁ"); break;
					case 0x2B: fprintf(pDUMP,"Çﬂ"); break;
					case 0x2C: fprintf(pDUMP,"Ç‡"); break;
					case 0x2D: fprintf(pDUMP,"Ç‚"); break;
					case 0x2E: fprintf(pDUMP,"Ç‰"); break;
					case 0x2F: fprintf(pDUMP,"ÇÊ"); break;
					case 0x30: fprintf(pDUMP,"ÇÁ"); break;
					case 0x31: fprintf(pDUMP,"ÇË"); break;
					case 0x32: fprintf(pDUMP,"ÇÈ"); break;
					case 0x33: fprintf(pDUMP,"ÇÍ"); break;
					case 0x34: fprintf(pDUMP,"ÇÎ"); break;
					case 0x35: fprintf(pDUMP,"ÇÌ"); break;
					case 0x36: fprintf(pDUMP,"ÇÒ"); break;
					case 0x37: fprintf(pDUMP,"Ç"); break;
					case 0x38: fprintf(pDUMP,"Ç¡"); break;
					case 0x39: fprintf(pDUMP,"Ç·"); break;
					case 0x3A: fprintf(pDUMP,"Ç„"); break;
					case 0x3B: fprintf(pDUMP,"ÇÂ"); break;
					case 0x3C: fprintf(pDUMP,"Çü"); break;
					case 0x3D: fprintf(pDUMP,"Ç°"); break;
					case 0x3E: fprintf(pDUMP,"Ç£"); break;
					case 0x3F: fprintf(pDUMP,"Ç•"); break;
					case 0x40: fprintf(pDUMP,"Çß"); break;
					case 0x41: fprintf(pDUMP,"Ç™"); break;
					case 0x42: fprintf(pDUMP,"Ç¨"); break;
					case 0x43: fprintf(pDUMP,"ÇÆ"); break;
					case 0x44: fprintf(pDUMP,"Ç∞"); break;
					case 0x45: fprintf(pDUMP,"Ç≤"); break;
					case 0x46: fprintf(pDUMP,"Ç¥"); break;
					case 0x47: fprintf(pDUMP,"Ç∂"); break;
					case 0x48: fprintf(pDUMP,"Ç∏"); break;
					case 0x49: fprintf(pDUMP,"Ç∫"); break;
					case 0x4A: fprintf(pDUMP,"Çº"); break;
					case 0x4B: fprintf(pDUMP,"Çæ"); break;
					case 0x4C: fprintf(pDUMP,"Ç¿"); break;
					case 0x4D: fprintf(pDUMP,"Ç√"); break;
					case 0x4E: fprintf(pDUMP,"Ç≈"); break;
					case 0x4F: fprintf(pDUMP,"Ç«"); break;
					case 0x50: fprintf(pDUMP,"ÇŒ"); break;
					case 0x51: fprintf(pDUMP,"Ç—"); break;
					case 0x52: fprintf(pDUMP,"Ç‘"); break;
					case 0x53: fprintf(pDUMP,"Ç◊"); break;
					case 0x54: fprintf(pDUMP,"Ç⁄"); break;
					case 0x55: fprintf(pDUMP,"Çœ"); break;
					case 0x56: fprintf(pDUMP,"Ç“"); break;
					case 0x57: fprintf(pDUMP,"Ç’"); break;
					case 0x58: fprintf(pDUMP,"Çÿ"); break;
					case 0x59: fprintf(pDUMP,"Ç€"); break;
					case 0x5A: fprintf(pDUMP,"Ç`"); break;
					case 0x5B: fprintf(pDUMP,"Ça"); break;
					case 0x5C: fprintf(pDUMP,"Çb"); break;
					case 0x5D: fprintf(pDUMP,"Çc"); break;
					case 0x5E: fprintf(pDUMP,"Çd"); break;
					case 0x5F: fprintf(pDUMP,"Çe"); break;
					case 0x60: fprintf(pDUMP,"Çf"); break;
					case 0x61: fprintf(pDUMP,"Çg"); break;
					case 0x62: fprintf(pDUMP,"Çh"); break;
					case 0x63: fprintf(pDUMP,"Çi"); break;
					case 0x64: fprintf(pDUMP,"Çj"); break;
					case 0x65: fprintf(pDUMP,"Çk"); break;
					case 0x66: fprintf(pDUMP,"Çl"); break;
					case 0x67: fprintf(pDUMP,"Çm"); break;
					case 0x68: fprintf(pDUMP,"Çn"); break;
					case 0x69: fprintf(pDUMP,"Ço"); break;
					case 0x6A: fprintf(pDUMP,"Çp"); break;
					case 0x6B: fprintf(pDUMP,"Çq"); break;
					case 0x6C: fprintf(pDUMP,"Çr"); break;
					case 0x6D: fprintf(pDUMP,"Çs"); break;
					case 0x6E: fprintf(pDUMP,"Çt"); break;
					case 0x6F: fprintf(pDUMP,"Çu"); break;
					case 0x70: fprintf(pDUMP,"Çv"); break;
					case 0x71: fprintf(pDUMP,"Çw"); break;
					case 0x72: fprintf(pDUMP,"Çx"); break;
					case 0x73: fprintf(pDUMP,"Çy"); break;
					case 0x74: fprintf(pDUMP,"ÅE"); break;
					case 0x75: fprintf(pDUMP,"ÅF"); break;
					case 0x76: fprintf(pDUMP,"ÅG"); break;
					case 0x77: fprintf(pDUMP,"Å`"); break;
					case 0x78: fprintf(pDUMP,"Å^"); break;
					case 0x79: fprintf(pDUMP,"Åi"); break;
					case 0x7A: fprintf(pDUMP,"Åj"); break;
					case 0x7B: fprintf(pDUMP,"êπ"); break;
					case 0x7C: fprintf(pDUMP,"åï"); break;
					case 0x7D: fprintf(pDUMP,"ì`"); break;
					case 0x7E: fprintf(pDUMP,"ê‡"); break;
					case 0x7F: fprintf(pDUMP,"Å@"); break;
					case 0x80: fprintf(pDUMP,"ÇO"); break;
					case 0x81: fprintf(pDUMP,"ÇP"); break;
					case 0x82: fprintf(pDUMP,"ÇQ"); break;
					case 0x83: fprintf(pDUMP,"ÇR"); break;
					case 0x84: fprintf(pDUMP,"ÇS"); break;
					case 0x85: fprintf(pDUMP,"ÇT"); break;
					case 0x86: fprintf(pDUMP,"ÇU"); break;
					case 0x87: fprintf(pDUMP,"ÇV"); break;
					case 0x88: fprintf(pDUMP,"ÇW"); break;
					case 0x89: fprintf(pDUMP,"ÇX"); break;
					case 0x8A: fprintf(pDUMP,"ÉA"); break;
					case 0x8B: fprintf(pDUMP,"ÉC"); break;
					case 0x8C: fprintf(pDUMP,"ÉE"); break;
					case 0x8D: fprintf(pDUMP,"ÉG"); break;
					case 0x8E: fprintf(pDUMP,"ÉI"); break;
					case 0x8F: fprintf(pDUMP,"ÉJ"); break;
					case 0x90: fprintf(pDUMP,"ÉL"); break;
					case 0x91: fprintf(pDUMP,"ÉN"); break;
					case 0x92: fprintf(pDUMP,"ÉP"); break;
					case 0x93: fprintf(pDUMP,"ÉR"); break;
					case 0x94: fprintf(pDUMP,"ÉT"); break;
					case 0x95: fprintf(pDUMP,"ÉV"); break;
					case 0x96: fprintf(pDUMP,"ÉX"); break;
					case 0x97: fprintf(pDUMP,"ÉZ"); break;
					case 0x98: fprintf(pDUMP,"É\\"); break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x99: fprintf(pDUMP,"É^"); break;
					case 0x9A: fprintf(pDUMP,"É`"); break;
					case 0x9B: fprintf(pDUMP,"Éc"); break;
					case 0x9C: fprintf(pDUMP,"Ée"); break;
					case 0x9D: fprintf(pDUMP,"Ég"); break;
					case 0x9E: fprintf(pDUMP,"Éi"); break;
					case 0x9F: fprintf(pDUMP,"Éj"); break;
					case 0xA0: fprintf(pDUMP,"Ék"); break;
					case 0xA1: fprintf(pDUMP,"Él"); break;
					case 0xA2: fprintf(pDUMP,"Ém"); break;
					case 0xA3: fprintf(pDUMP,"Én"); break;
					case 0xA4: fprintf(pDUMP,"Éq"); break;
					case 0xA5: fprintf(pDUMP,"Ét"); break;
					case 0xA6: fprintf(pDUMP,"Éw"); break;
					case 0xA7: fprintf(pDUMP,"Éz"); break;
					case 0xA8: fprintf(pDUMP,"É}"); break;
					case 0xA9: fprintf(pDUMP,"É~"); break;
					case 0xAA: fprintf(pDUMP,"ÉÄ"); break;
					case 0xAB: fprintf(pDUMP,"ÉÅ"); break;
					case 0xAC: fprintf(pDUMP,"ÉÇ"); break;
					case 0xAD: fprintf(pDUMP,"ÉÑ"); break;
					case 0xAE: fprintf(pDUMP,"ÉÜ"); break;
					case 0xAF: fprintf(pDUMP,"Éà"); break;
					case 0xB0: fprintf(pDUMP,"Éâ"); break;
					case 0xB1: fprintf(pDUMP,"Éä"); break;
					case 0xB2: fprintf(pDUMP,"Éã"); break;
					case 0xB3: fprintf(pDUMP,"Éå"); break;
					case 0xB4: fprintf(pDUMP,"Éç"); break;
					case 0xB5: fprintf(pDUMP,"Éè"); break;
					case 0xB6: fprintf(pDUMP,"Éì"); break;
					case 0xB7: fprintf(pDUMP,"Éí"); break;
					case 0xB8: fprintf(pDUMP,"Éb"); break;
					case 0xB9: fprintf(pDUMP,"ÉÉ"); break;
					case 0xBA: fprintf(pDUMP,"ÉÖ"); break;
					case 0xBB: fprintf(pDUMP,"Éá"); break;
					case 0xBC: fprintf(pDUMP,"É@"); break;
					case 0xBD: fprintf(pDUMP,"ÉB"); break;
					case 0xBE: fprintf(pDUMP,"ÉD"); break;
					case 0xBF: fprintf(pDUMP,"ÉF"); break;
					case 0xC0: fprintf(pDUMP,"ÉH"); break;
					case 0xC1: fprintf(pDUMP,"ÉK"); break;
					case 0xC2: fprintf(pDUMP,"ÉM"); break;
					case 0xC3: fprintf(pDUMP,"ÉO"); break;
					case 0xC4: fprintf(pDUMP,"ÉQ"); break;
					case 0xC5: fprintf(pDUMP,"ÉS"); break;
					case 0xC6: fprintf(pDUMP,"ÉU"); break;
					case 0xC7: fprintf(pDUMP,"ÉW"); break;
					case 0xC8: fprintf(pDUMP,"ÉY"); break;
					case 0xC9: fprintf(pDUMP,"É["); break;
					case 0xCA: fprintf(pDUMP,"É]"); break;
					case 0xCB: fprintf(pDUMP,"É_"); break;
					case 0xCC: fprintf(pDUMP,"Éa"); break;
					case 0xCD: fprintf(pDUMP,"Éd"); break;
					case 0xCE: fprintf(pDUMP,"Éf"); break;
					case 0xCF: fprintf(pDUMP,"Éh"); break;
					case 0xD0: fprintf(pDUMP,"Éo"); break;
					case 0xD1: fprintf(pDUMP,"Ér"); break;
					case 0xD2: fprintf(pDUMP,"Éu"); break;
					case 0xD3: fprintf(pDUMP,"Éx"); break;
					case 0xD4: fprintf(pDUMP,"É{"); break;
					case 0xD5: fprintf(pDUMP,"Ép"); break;
					case 0xD6: fprintf(pDUMP,"És"); break;
					case 0xD7: fprintf(pDUMP,"Év"); break;
					case 0xD8: fprintf(pDUMP,"Éy"); break;
					case 0xD9: fprintf(pDUMP,"É|"); break;
					case 0xDA: fprintf(pDUMP,"ÇÅ"); break;
					case 0xDB: fprintf(pDUMP,"ÇÇ"); break;
					case 0xDC: fprintf(pDUMP,"ÇÉ"); break;
					case 0xDD: fprintf(pDUMP,"ÇÑ"); break;
					case 0xDE: fprintf(pDUMP,"ÇÖ"); break;
					case 0xDF: fprintf(pDUMP,"ÇÜ"); break;
					case 0xE0: fprintf(pDUMP,"Çá"); break;
					case 0xE1: fprintf(pDUMP,"Çà"); break;
					case 0xE2: fprintf(pDUMP,"Çâ"); break;
					case 0xE3: fprintf(pDUMP,"Çä"); break;
					case 0xE4: fprintf(pDUMP,"Çã"); break;
					case 0xE5: fprintf(pDUMP,"Çå"); break;
					case 0xE6: fprintf(pDUMP,"Çç"); break;
					case 0xE7: fprintf(pDUMP,"Çé"); break;
					case 0xE8: fprintf(pDUMP,"Çè"); break;
					case 0xE9: fprintf(pDUMP,"Çê"); break;
					case 0xEA: fprintf(pDUMP,"Çë"); break;
					case 0xEB: fprintf(pDUMP,"Çí"); break;
					case 0xEC: fprintf(pDUMP,"Çì"); break;
					case 0xED: fprintf(pDUMP,"Çî"); break;
					case 0xEE: fprintf(pDUMP,"Çï"); break;
					case 0xEF: fprintf(pDUMP,"Çñ"); break;
					case 0xF0: fprintf(pDUMP,"Çó"); break;
					case 0xF1: fprintf(pDUMP,"Çò"); break;
					case 0xF2: fprintf(pDUMP,"Çô"); break;
					case 0xF3: fprintf(pDUMP,"Çö"); break;
					case 0xF4: fprintf(pDUMP,"Éî"); break;
					case 0xF5: fprintf(pDUMP,"ÅX"); break;
					case 0xF6: fprintf(pDUMP,"óé"); break;
					case 0xF7: fprintf(pDUMP,"à´"); break;
					case 0xF8: fprintf(pDUMP,"ì¸"); break;
					case 0xF9: fprintf(pDUMP,"éË"); break;
					case 0xFA: fprintf(pDUMP,"óp"); break;
					case 0xFB: fprintf(pDUMP,"êÊ"); break;
					case 0xFC: fprintf(pDUMP,"à¿"); break;
					case 0xFD: fprintf(pDUMP,"íN"); break;
					case 0xFE: fprintf(pDUMP,"âπ"); break;
					case 0xFF: fprintf(pDUMP,"Å§"); break;
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
					case 0x0: fprintf(pDUMP,"ÇO"); break;
					case 0x1: fprintf(pDUMP,"ÇP"); break;
					case 0x2: fprintf(pDUMP,"ÇQ"); break;
					case 0x3: fprintf(pDUMP,"ÇR"); break;
					case 0x4: fprintf(pDUMP,"ÇS"); break;
					case 0x5: fprintf(pDUMP,"ÇT"); break;
					case 0x6: fprintf(pDUMP,"ÇU"); break;
					case 0x7: fprintf(pDUMP,"ÇV"); break;
					case 0x8: fprintf(pDUMP,"ÇW"); break;
					case 0x9: fprintf(pDUMP,"ÇX"); break;
					case 0x0A: fprintf(pDUMP,"ÉA"); break;
					case 0x0B: fprintf(pDUMP,"ÉC"); break;
					case 0x0C: fprintf(pDUMP,"ÉE"); break;
					case 0x0D: fprintf(pDUMP,"ÉG"); break;
					case 0x0E: fprintf(pDUMP,"ÉI"); break;
					case 0x0F: fprintf(pDUMP,"ÉJ"); break;
					case 0x10: fprintf(pDUMP,"ÉL"); break;
					case 0x11: fprintf(pDUMP,"ÉN"); break;
					case 0x12: fprintf(pDUMP,"ÉP"); break;
					case 0x13: fprintf(pDUMP,"ÉR"); break;
					case 0x14: fprintf(pDUMP,"ÉT"); break;
					case 0x15: fprintf(pDUMP,"ÉV"); break;
					case 0x16: fprintf(pDUMP,"ÉX"); break;
					case 0x17: fprintf(pDUMP,"ÉZ"); break;
					case 0x18: fprintf(pDUMP,"É\\"); break;//wierdness here with katakana 'so'. Had to add a '\'
					case 0x19: fprintf(pDUMP,"É^"); break;
					case 0x1A: fprintf(pDUMP,"É`"); break;
					case 0x1B: fprintf(pDUMP,"Éc"); break;
					case 0x1C: fprintf(pDUMP,"Ée"); break;
					case 0x1D: fprintf(pDUMP,"Ég"); break;
					case 0x1E: fprintf(pDUMP,"Éi"); break;
					case 0x1F: fprintf(pDUMP,"Éj"); break;
					case 0x20: fprintf(pDUMP,"Ék"); break;
					case 0x21: fprintf(pDUMP,"Él"); break;
					case 0x22: fprintf(pDUMP,"Ém"); break;
					case 0x23: fprintf(pDUMP,"Én"); break;
					case 0x24: fprintf(pDUMP,"Éq"); break;
					case 0x25: fprintf(pDUMP,"Ét"); break;
					case 0x26: fprintf(pDUMP,"Éw"); break;
					case 0x27: fprintf(pDUMP,"Éz"); break;
					case 0x28: fprintf(pDUMP,"É}"); break;
					case 0x29: fprintf(pDUMP,"É~"); break;
					case 0x2A: fprintf(pDUMP,"ÉÄ"); break;
					case 0x2B: fprintf(pDUMP,"ÉÅ"); break;
					case 0x2C: fprintf(pDUMP,"ÉÇ"); break;
					case 0x2D: fprintf(pDUMP,"ÉÑ"); break;
					case 0x2E: fprintf(pDUMP,"ÉÜ"); break;
					case 0x2F: fprintf(pDUMP,"Éà"); break;
					case 0x30: fprintf(pDUMP,"Éâ"); break;
					case 0x31: fprintf(pDUMP,"Éä"); break;
					case 0x32: fprintf(pDUMP,"Éã"); break;
					case 0x33: fprintf(pDUMP,"Éå"); break;
					case 0x34: fprintf(pDUMP,"Éç"); break;
					case 0x35: fprintf(pDUMP,"Éè"); break;
					case 0x36: fprintf(pDUMP,"Éì"); break;
					case 0x37: fprintf(pDUMP,"Éí"); break;
					case 0x38: fprintf(pDUMP,"Éb"); break;
					case 0x39: fprintf(pDUMP,"ÉÉ"); break;
					case 0x3A: fprintf(pDUMP,"ÉÖ"); break;
					case 0x3B: fprintf(pDUMP,"Éá"); break;
					case 0x3C: fprintf(pDUMP,"É@"); break;
					case 0x3D: fprintf(pDUMP,"ÉB"); break;
					case 0x3E: fprintf(pDUMP,"ÉD"); break;
					case 0x3F: fprintf(pDUMP,"ÉF"); break;
					case 0x40: fprintf(pDUMP,"ÉH"); break;
					case 0x41: fprintf(pDUMP,"ÉK"); break;
					case 0x42: fprintf(pDUMP,"ÉM"); break;
					case 0x43: fprintf(pDUMP,"ÉO"); break;
					case 0x44: fprintf(pDUMP,"ÉQ"); break;
					case 0x45: fprintf(pDUMP,"ÉS"); break;
					case 0x46: fprintf(pDUMP,"ÉU"); break;
					case 0x47: fprintf(pDUMP,"ÉW"); break;
					case 0x48: fprintf(pDUMP,"ÉY"); break;
					case 0x49: fprintf(pDUMP,"É["); break;
					case 0x4A: fprintf(pDUMP,"É]"); break;
					case 0x4B: fprintf(pDUMP,"É_"); break;
					case 0x4C: fprintf(pDUMP,"Éa"); break;
					case 0x4D: fprintf(pDUMP,"Éd"); break;
					case 0x4E: fprintf(pDUMP,"Éf"); break;
					case 0x4F: fprintf(pDUMP,"Éh"); break;
					case 0x50: fprintf(pDUMP,"Éo"); break;
					case 0x51: fprintf(pDUMP,"Ér"); break;
					case 0x52: fprintf(pDUMP,"Éu"); break;
					case 0x53: fprintf(pDUMP,"Éx"); break;
					case 0x54: fprintf(pDUMP,"É{"); break;
					case 0x55: fprintf(pDUMP,"Ép"); break;
					case 0x56: fprintf(pDUMP,"És"); break;
					case 0x57: fprintf(pDUMP,"Év"); break;
					case 0x58: fprintf(pDUMP,"Éy"); break;
					case 0x59: fprintf(pDUMP,"É|"); break;
					case 0x5A: fprintf(pDUMP,"ÇÅ"); break;
					case 0x5B: fprintf(pDUMP,"ÇÇ"); break;
					case 0x5C: fprintf(pDUMP,"ÇÉ"); break;
					case 0x5D: fprintf(pDUMP,"ÇÑ"); break;
					case 0x5E: fprintf(pDUMP,"ÇÖ"); break;
					case 0x5F: fprintf(pDUMP,"ÇÜ"); break;
					case 0x60: fprintf(pDUMP,"Çá"); break;
					case 0x61: fprintf(pDUMP,"Çà"); break;
					case 0x62: fprintf(pDUMP,"Çâ"); break;
					case 0x63: fprintf(pDUMP,"Çä"); break;
					case 0x64: fprintf(pDUMP,"Çã"); break;
					case 0x65: fprintf(pDUMP,"Çå"); break;
					case 0x66: fprintf(pDUMP,"Çç"); break;
					case 0x67: fprintf(pDUMP,"Çé"); break;
					case 0x68: fprintf(pDUMP,"Çè"); break;
					case 0x69: fprintf(pDUMP,"Çê"); break;
					case 0x6A: fprintf(pDUMP,"Çë"); break;
					case 0x6B: fprintf(pDUMP,"Çí"); break;
					case 0x6C: fprintf(pDUMP,"Çì"); break;
					case 0x6D: fprintf(pDUMP,"Çî"); break;
					case 0x6E: fprintf(pDUMP,"Çï"); break;
					case 0x6F: fprintf(pDUMP,"Çñ"); break;
					case 0x70: fprintf(pDUMP,"Çó"); break;
					case 0x71: fprintf(pDUMP,"Çò"); break;
					case 0x72: fprintf(pDUMP,"Çô"); break;
					case 0x73: fprintf(pDUMP,"Çö"); break;
					case 0x74: fprintf(pDUMP,"Éî"); break;
					case 0x75: fprintf(pDUMP,"ÅX"); break;
					case 0x76: fprintf(pDUMP,"óé"); break;
					case 0x77: fprintf(pDUMP,"à´"); break;
					case 0x78: fprintf(pDUMP,"ì¸"); break;
					case 0x79: fprintf(pDUMP,"éË"); break;
					case 0x7A: fprintf(pDUMP,"óp"); break;
					case 0x7B: fprintf(pDUMP,"êÊ"); break;
					case 0x7C: fprintf(pDUMP,"à¿"); break;
					case 0x7D: fprintf(pDUMP,"íN"); break;
					case 0x7E: fprintf(pDUMP,"âπ"); break;
					case 0x7F: fprintf(pDUMP,"Å§"); break;
					case 0x80: fprintf(pDUMP,"éÂ"); break;
					case 0x81: fprintf(pDUMP,"êl"); break;
					case 0x82: fprintf(pDUMP,"í¨"); break;
					case 0x83: fprintf(pDUMP,"êX"); break;
					case 0x84: fprintf(pDUMP,"óà"); break;
					case 0x85: fprintf(pDUMP,"ä‘"); break;
					case 0x86: fprintf(pDUMP,"év"); break;
					case 0x87: fprintf(pDUMP,"çá"); break;
					case 0x88: fprintf(pDUMP,"åı"); break;
					case 0x89: fprintf(pDUMP,"è¡"); break;
					case 0x8A: fprintf(pDUMP,"å`"); break;
					case 0x8B: fprintf(pDUMP,"âÒ"); break;
					case 0x8C: fprintf(pDUMP,"ç¢"); break;
					case 0x8D: fprintf(pDUMP,"í∑"); break;
					case 0x8E: fprintf(pDUMP,"òV"); break;
					case 0x8F: fprintf(pDUMP,"ìå"); break;
					case 0x90: fprintf(pDUMP,"êº"); break;
					case 0x91: fprintf(pDUMP,"ó"); break;
					case 0x92: fprintf(pDUMP,"éj"); break;
					case 0x93: fprintf(pDUMP,"âÔ"); break;
					case 0x94: fprintf(pDUMP,"ä÷"); break;
					case 0x95: fprintf(pDUMP,"åW"); break;
					case 0x96: fprintf(pDUMP,"ê^"); break;
					case 0x97: fprintf(pDUMP,"ñk"); break;
					case 0x98: fprintf(pDUMP,"ìÏ"); break;
					case 0x99: fprintf(pDUMP,"ëÂ"); break;
					case 0x9A: fprintf(pDUMP,"ín"); break;
					case 0x9B: fprintf(pDUMP,"êŒ"); break;
					case 0x9C: fprintf(pDUMP,"ñ⁄"); break;
					case 0x9D: fprintf(pDUMP,"âΩ"); break;
					case 0x9E: fprintf(pDUMP,"ó—"); break;
					case 0x9F: fprintf(pDUMP,"í "); break;
					case 0xA0: fprintf(pDUMP,"íj"); break;
					case 0xA1: fprintf(pDUMP,"å„"); break;
					case 0xA2: fprintf(pDUMP,"ìX"); break;
					case 0xA3: fprintf(pDUMP,"ñ∫"); break;
					case 0xA4: fprintf(pDUMP,"èó"); break;
					case 0xA5: fprintf(pDUMP,"ãü"); break;
					case 0xA6: fprintf(pDUMP,"íÈ"); break;
					case 0xA7: fprintf(pDUMP,"çë"); break;
					case 0xA8: fprintf(pDUMP,"éÑ"); break;
					case 0xA9: fprintf(pDUMP,"ãq"); break;
					case 0xAA: fprintf(pDUMP,"âª"); break;
					case 0xAB: fprintf(pDUMP,"çs"); break;
					case 0xAC: fprintf(pDUMP,"ï˚"); break;
					case 0xAD: fprintf(pDUMP,"ñæ"); break;
					case 0xAE: fprintf(pDUMP,"çc"); break;
					case 0xAF: fprintf(pDUMP,"óv"); break;
					case 0xB0: fprintf(pDUMP,"ç«"); break;
					case 0xB1: fprintf(pDUMP,"âJ"); break;
					case 0xB2: fprintf(pDUMP,"ñÿ"); break;
					case 0xB3: fprintf(pDUMP,"ñ≈"); break;
					case 0xB4: fprintf(pDUMP,"åN"); break;
					case 0xB5: fprintf(pDUMP,"Åì"); break;
					case 0xB6: fprintf(pDUMP,"ê∂"); break;
					case 0xB7: fprintf(pDUMP,"åå"); break;
					case 0xB8: fprintf(pDUMP,"ç≤"); break;
					case 0xB9: fprintf(pDUMP,"éÁ"); break;
					case 0xBA: fprintf(pDUMP,"êÌ"); break;
					case 0xBB: fprintf(pDUMP,"ëà"); break;
					case 0xBC: fprintf(pDUMP,"ãR"); break;
					case 0xBD: fprintf(pDUMP,"ém"); break;
					case 0xBE: fprintf(pDUMP,"ìG"); break;
					case 0xBF: fprintf(pDUMP,"åä"); break;
					case 0xC0: fprintf(pDUMP,"å¸"); break;
					case 0xC1: fprintf(pDUMP,"à¯"); break;
					case 0xC2: fprintf(pDUMP,"é◊"); break;
					case 0xC3: fprintf(pDUMP,"ìÆ"); break;
					case 0xC4: fprintf(pDUMP,"ê¨"); break;
					case 0xC5: fprintf(pDUMP,"å˜"); break;
					case 0xC6: fprintf(pDUMP,"ïv"); break;
					case 0xC7: fprintf(pDUMP,"ï∂"); break;
					case 0xC8: fprintf(pDUMP,"é·"); break;
					case 0xC9: fprintf(pDUMP,"ó¨"); break;
					case 0xCA: fprintf(pDUMP,"é∏"); break;
					case 0xCB: fprintf(pDUMP,"êÖ"); break;
					case 0xCC: fprintf(pDUMP,"äO"); break;
					case 0xCD: fprintf(pDUMP,"é©"); break;
					case 0xCE: fprintf(pDUMP,"ï™"); break;
					case 0xCF: fprintf(pDUMP,"êg"); break;
					case 0xD0: fprintf(pDUMP,"ñ{"); break;
					case 0xD1: fprintf(pDUMP,"é„"); break;
					case 0xD2: fprintf(pDUMP,"é∏"); break;
					case 0xD3: fprintf(pDUMP,"äB"); break;
					case 0xD4: fprintf(pDUMP,"å´"); break;
					case 0xD5: fprintf(pDUMP,"é“"); break;
					case 0xD6: fprintf(pDUMP,"äC"); break;
					case 0xD7: fprintf(pDUMP,"ìá"); break;
					case 0xD8: fprintf(pDUMP,"òb"); break;
					case 0xD9: fprintf(pDUMP,"äE"); break;
					case 0xDA: fprintf(pDUMP,"ëS"); break;
					case 0xDB: fprintf(pDUMP,"éÄ"); break;
					case 0xDC: fprintf(pDUMP,"çD"); break;
					case 0xDD: fprintf(pDUMP,"ç–"); break;
					case 0xDE: fprintf(pDUMP,"å√"); break;
					case 0xDF: fprintf(pDUMP,"ë„"); break;
					case 0xE0: fprintf(pDUMP,"ä‚"); break;
					case 0xE1: fprintf(pDUMP,"ïÛ"); break;
					case 0xE2: fprintf(pDUMP,"ïÉ"); break;
					case 0xE3: fprintf(pDUMP,"îN"); break;
					case 0xE4: fprintf(pDUMP,"ìa"); break;
					case 0xE5: fprintf(pDUMP,"ëê"); break;
					case 0xE6: fprintf(pDUMP,"çÇ"); break;
					case 0xE7: fprintf(pDUMP,"ã≠"); break;
					case 0xE8: fprintf(pDUMP,"êÿ"); break;
					case 0xE9: fprintf(pDUMP,"íá"); break;
					case 0xEA: fprintf(pDUMP,"ån"); break;
					case 0xEB: fprintf(pDUMP,"îí"); break;
					case 0xEC: fprintf(pDUMP,"ì≈"); break;
					case 0xED: fprintf(pDUMP,"é_"); break;
					case 0xEE: fprintf(pDUMP,"ï®"); break;
					case 0xEF: fprintf(pDUMP,"ïú"); break;
					case 0xF0: fprintf(pDUMP,"ì_"); break;
					case 0xF1: fprintf(pDUMP,"ñ∞"); break;
					case 0xF2: fprintf(pDUMP,"âF"); break;
					case 0xF3: fprintf(pDUMP,"íà"); break;
					case 0xF4: fprintf(pDUMP,"ó§"); break;
					case 0xF5: fprintf(pDUMP,"è≠"); break;
					case 0xF6: fprintf(pDUMP,"éR"); break;
					case 0xF7: fprintf(pDUMP,"éw"); break;
					case 0xF8: fprintf(pDUMP,"ëÍ"); break;
					case 0xF9: fprintf(pDUMP,"éñ"); break;
					case 0xFA: fprintf(pDUMP,"ìñ"); break;
					case 0xFB: fprintf(pDUMP,"MP"); break;
					case 0xFC: fprintf(pDUMP,"Å™"); break;
					case 0xFD: fprintf(pDUMP,"Å´"); break;
					case 0xFE: fprintf(pDUMP,"Å©"); break;
					case 0xFF: fprintf(pDUMP,"Å®"); break;
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
					case 0x0: fprintf(pDUMP,"éÂ"); break;
					case 0x1: fprintf(pDUMP,"êl"); break;
					case 0x2: fprintf(pDUMP,"í¨"); break;
					case 0x3: fprintf(pDUMP,"êX"); break;
					case 0x4: fprintf(pDUMP,"óà"); break;
					case 0x5: fprintf(pDUMP,"ä‘"); break;
					case 0x6: fprintf(pDUMP,"év"); break;
					case 0x7: fprintf(pDUMP,"çá"); break;
					case 0x8: fprintf(pDUMP,"åı"); break;
					case 0x9: fprintf(pDUMP,"è¡"); break;
					case 0x0A: fprintf(pDUMP,"å`"); break;
					case 0x0B: fprintf(pDUMP,"âÒ"); break;
					case 0x0C: fprintf(pDUMP,"ç¢"); break;
					case 0x0D: fprintf(pDUMP,"í∑"); break;
					case 0x0E: fprintf(pDUMP,"òV"); break;
					case 0x0F: fprintf(pDUMP,"ìå"); break;
					case 0x10: fprintf(pDUMP,"êº"); break;
					case 0x11: fprintf(pDUMP,"ó"); break;
					case 0x12: fprintf(pDUMP,"éj"); break;
					case 0x13: fprintf(pDUMP,"âÔ"); break;
					case 0x14: fprintf(pDUMP,"ä÷"); break;
					case 0x15: fprintf(pDUMP,"åW"); break;
					case 0x16: fprintf(pDUMP,"ê^"); break;
					case 0x17: fprintf(pDUMP,"ñk"); break;
					case 0x18: fprintf(pDUMP,"ìÏ"); break;
					case 0x19: fprintf(pDUMP,"ëÂ"); break;
					case 0x1A: fprintf(pDUMP,"ín"); break;
					case 0x1B: fprintf(pDUMP,"êŒ"); break;
					case 0x1C: fprintf(pDUMP,"ñ⁄"); break;
					case 0x1D: fprintf(pDUMP,"âΩ"); break;
					case 0x1E: fprintf(pDUMP,"ó—"); break;
					case 0x1F: fprintf(pDUMP,"í "); break;
					case 0x20: fprintf(pDUMP,"íj"); break;
					case 0x21: fprintf(pDUMP,"å„"); break;
					case 0x22: fprintf(pDUMP,"ìX"); break;
					case 0x23: fprintf(pDUMP,"ñ∫"); break;
					case 0x24: fprintf(pDUMP,"èó"); break;
					case 0x25: fprintf(pDUMP,"ãü"); break;
					case 0x26: fprintf(pDUMP,"íÈ"); break;
					case 0x27: fprintf(pDUMP,"çë"); break;
					case 0x28: fprintf(pDUMP,"éÑ"); break;
					case 0x29: fprintf(pDUMP,"ãq"); break;
					case 0x2A: fprintf(pDUMP,"âª"); break;
					case 0x2B: fprintf(pDUMP,"çs"); break;
					case 0x2C: fprintf(pDUMP,"ï˚"); break;
					case 0x2D: fprintf(pDUMP,"ñæ"); break;
					case 0x2E: fprintf(pDUMP,"çc"); break;
					case 0x2F: fprintf(pDUMP,"óv"); break;
					case 0x30: fprintf(pDUMP,"ç«"); break;
					case 0x31: fprintf(pDUMP,"âJ"); break;
					case 0x32: fprintf(pDUMP,"ñÿ"); break;
					case 0x33: fprintf(pDUMP,"ñ≈"); break;
					case 0x34: fprintf(pDUMP,"åN"); break;
					case 0x35: fprintf(pDUMP,"Åì"); break;
					case 0x36: fprintf(pDUMP,"ê∂"); break;
					case 0x37: fprintf(pDUMP,"åå"); break;
					case 0x38: fprintf(pDUMP,"ç≤"); break;
					case 0x39: fprintf(pDUMP,"éÁ"); break;
					case 0x3A: fprintf(pDUMP,"êÌ"); break;
					case 0x3B: fprintf(pDUMP,"ëà"); break;
					case 0x3C: fprintf(pDUMP,"ãR"); break;
					case 0x3D: fprintf(pDUMP,"ém"); break;
					case 0x3E: fprintf(pDUMP,"ìG"); break;
					case 0x3F: fprintf(pDUMP,"åä"); break;
					case 0x40: fprintf(pDUMP,"å¸"); break;
					case 0x41: fprintf(pDUMP,"à¯"); break;
					case 0x42: fprintf(pDUMP,"é◊"); break;
					case 0x43: fprintf(pDUMP,"ìÆ"); break;
					case 0x44: fprintf(pDUMP,"ê¨"); break;
					case 0x45: fprintf(pDUMP,"å˜"); break;
					case 0x46: fprintf(pDUMP,"ïv"); break;
					case 0x47: fprintf(pDUMP,"ï∂"); break;
					case 0x48: fprintf(pDUMP,"é·"); break;
					case 0x49: fprintf(pDUMP,"ó¨"); break;
					case 0x4A: fprintf(pDUMP,"é∏"); break;
					case 0x4B: fprintf(pDUMP,"êÖ"); break;
					case 0x4C: fprintf(pDUMP,"äO"); break;
					case 0x4D: fprintf(pDUMP,"é©"); break;
					case 0x4E: fprintf(pDUMP,"ï™"); break;
					case 0x4F: fprintf(pDUMP,"êg"); break;
					case 0x50: fprintf(pDUMP,"ñ{"); break;
					case 0x51: fprintf(pDUMP,"é„"); break;
					case 0x52: fprintf(pDUMP,"é∏"); break;
					case 0x53: fprintf(pDUMP,"äB"); break;
					case 0x54: fprintf(pDUMP,"å´"); break;
					case 0x55: fprintf(pDUMP,"é“"); break;
					case 0x56: fprintf(pDUMP,"äC"); break;
					case 0x57: fprintf(pDUMP,"ìá"); break;
					case 0x58: fprintf(pDUMP,"òb"); break;
					case 0x59: fprintf(pDUMP,"äE"); break;
					case 0x5A: fprintf(pDUMP,"ëS"); break;
					case 0x5B: fprintf(pDUMP,"éÄ"); break;
					case 0x5C: fprintf(pDUMP,"çD"); break;
					case 0x5D: fprintf(pDUMP,"ç–"); break;
					case 0x5E: fprintf(pDUMP,"å√"); break;
					case 0x5F: fprintf(pDUMP,"ë„"); break;
					case 0x60: fprintf(pDUMP,"ä‚"); break;
					case 0x61: fprintf(pDUMP,"ïÛ"); break;
					case 0x62: fprintf(pDUMP,"ïÉ"); break;
					case 0x63: fprintf(pDUMP,"îN"); break;
					case 0x64: fprintf(pDUMP,"ìa"); break;
					case 0x65: fprintf(pDUMP,"ëê"); break;
					case 0x66: fprintf(pDUMP,"çÇ"); break;
					case 0x67: fprintf(pDUMP,"ã≠"); break;
					case 0x68: fprintf(pDUMP,"êÿ"); break;
					case 0x69: fprintf(pDUMP,"íá"); break;
					case 0x6A: fprintf(pDUMP,"ån"); break;
					case 0x6B: fprintf(pDUMP,"îí"); break;
					case 0x6C: fprintf(pDUMP,"ì≈"); break;
					case 0x6D: fprintf(pDUMP,"é_"); break;
					case 0x6E: fprintf(pDUMP,"ï®"); break;
					case 0x6F: fprintf(pDUMP,"ïú"); break;
					case 0x70: fprintf(pDUMP,"ì_"); break;
					case 0x71: fprintf(pDUMP,"ñ∞"); break;
					case 0x72: fprintf(pDUMP,"âF"); break;
					case 0x73: fprintf(pDUMP,"íà"); break;
					case 0x74: fprintf(pDUMP,"ó§"); break;
					case 0x75: fprintf(pDUMP,"è≠"); break;
					case 0x76: fprintf(pDUMP,"éR"); break;
					case 0x77: fprintf(pDUMP,"éw"); break;
					case 0x78: fprintf(pDUMP,"ëÍ"); break;
					case 0x79: fprintf(pDUMP,"éñ"); break;
					case 0x7A: fprintf(pDUMP,"ìñ"); break;
					case 0x7B: fprintf(pDUMP,"MP"); break;
					case 0x7C: fprintf(pDUMP,"Å™"); break;
					case 0x7D: fprintf(pDUMP,"Å´"); break;
					case 0x7E: fprintf(pDUMP,"Å©"); break;
					case 0x7F: fprintf(pDUMP,"Å®"); break;
					case 0x80: fprintf(pDUMP,"âü"); break;
					case 0x81: fprintf(pDUMP,"à»"); break;
					case 0x82: fprintf(pDUMP,"èÈ"); break;
					case 0x83: fprintf(pDUMP,"éö"); break;
					case 0x84: fprintf(pDUMP,"è\\"); break;//more wierdness
					case 0x85: fprintf(pDUMP,"åà"); break;
					case 0x86: fprintf(pDUMP,"ëI"); break;
					case 0x87: fprintf(pDUMP,"íË"); break;
					case 0x88: fprintf(pDUMP,"éû"); break;
					case 0x89: fprintf(pDUMP,"ç∂"); break;
					case 0x8A: fprintf(pDUMP,"âE"); break;
					case 0x8B: fprintf(pDUMP,"êi"); break;
					case 0x8C: fprintf(pDUMP,"éü"); break;
					case 0x8D: fprintf(pDUMP,"ñﬂ"); break;
					case 0x8E: fprintf(pDUMP,"ñÇ"); break;
					case 0x8F: fprintf(pDUMP,"å©"); break;
					case 0x90: fprintf(pDUMP,"ñ@"); break;
					case 0x91: fprintf(pDUMP,"óÕ"); break;
					case 0x92: fprintf(pDUMP,"êl"); break;
					case 0x93: fprintf(pDUMP,"äÌ"); break;
					case 0x94: fprintf(pDUMP,"î"); break;
					case 0x95: fprintf(pDUMP,"ïê"); break;
					case 0x96: fprintf(pDUMP,"ïœ"); break;
					case 0x97: fprintf(pDUMP,"äe"); break;
					case 0x98: fprintf(pDUMP,"çU"); break;
					case 0x99: fprintf(pDUMP,"én"); break;
					case 0x9A: fprintf(pDUMP,"åÇ"); break;
					case 0x9B: fprintf(pDUMP,"è¡"); break;
					case 0x9C: fprintf(pDUMP,"âÒ"); break;
					case 0x9D: fprintf(pDUMP,"èI"); break;
					case 0x9E: fprintf(pDUMP,"ñh"); break;
					case 0x9F: fprintf(pDUMP,"å‰"); break;
					case 0xA0: fprintf(pDUMP,"âÆ"); break;
					case 0xA1: fprintf(pDUMP,"àÍ"); break;
					case 0xA2: fprintf(pDUMP,"åª"); break;
					case 0xA3: fprintf(pDUMP,"ç≈"); break;
					case 0xA4: fprintf(pDUMP,"èâ"); break;
					case 0xA5: fprintf(pDUMP,"ìØ"); break;
					case 0xA6: fprintf(pDUMP,"è„"); break;
					case 0xA7: fprintf(pDUMP,"â∫"); break;
					case 0xA8: fprintf(pDUMP,"ãZ"); break;
					case 0xA9: fprintf(pDUMP,"ñ "); break;
					case 0xAA: fprintf(pDUMP,"ã§"); break;
					case 0xAB: fprintf(pDUMP,"ó∑"); break;
					case 0xAC: fprintf(pDUMP,"ñ¬"); break;
					case 0xAD: fprintf(pDUMP,"ïΩ"); break;
					case 0xAE: fprintf(pDUMP,"ê∏"); break;
					case 0xAF: fprintf(pDUMP,"òa"); break;
					case 0xB0: fprintf(pDUMP,"ím"); break;
					case 0xB1: fprintf(pDUMP,"ëÃ"); break;
					case 0xB2: fprintf(pDUMP,"ïX"); break;
					case 0xB3: fprintf(pDUMP,"ëD"); break;
					case 0xB4: fprintf(pDUMP,"íÜ"); break;
					case 0xB5: fprintf(pDUMP,"ê_"); break;
					case 0xB6: fprintf(pDUMP,"ñΩ"); break;
					case 0xB7: fprintf(pDUMP,"ëO"); break;
					case 0xB8: fprintf(pDUMP,"è¨"); break;
					case 0xB9: fprintf(pDUMP,"ëΩ"); break;
					case 0xBA: fprintf(pDUMP,"ê¢"); break;
					case 0xBB: fprintf(pDUMP,"çÏ"); break;
					case 0xBC: fprintf(pDUMP,"ë±"); break;
					case 0xBD: fprintf(pDUMP,"äE"); break;
					case 0xBE: fprintf(pDUMP,"èb"); break;
					case 0xBF: fprintf(pDUMP,"ã‡"); break;
					case 0xC0: fprintf(pDUMP,"ã@"); break;
					case 0xC1: fprintf(pDUMP,"ê⁄"); break;
					case 0xC2: fprintf(pDUMP,"ãﬂ"); break;
					case 0xC3: fprintf(pDUMP,"ë“"); break;
					case 0xC4: fprintf(pDUMP,"ñº"); break;
					case 0xC5: fprintf(pDUMP,"óE"); break;
					case 0xC6: fprintf(pDUMP,"ãC"); break;
					case 0xC7: fprintf(pDUMP,"à§"); break;
					case 0xC8: fprintf(pDUMP,"åb"); break;
					case 0xC9: fprintf(pDUMP,"êS"); break;
					case 0xCA: fprintf(pDUMP,"çª"); break;
					case 0xCB: fprintf(pDUMP,"îô"); break;
					case 0xCC: fprintf(pDUMP,"íÍ"); break;
					case 0xCD: fprintf(pDUMP,"êÃ"); break;
					case 0xCE: fprintf(pDUMP,"êU"); break;
					case 0xCF: fprintf(pDUMP,"ãN"); break;
					case 0xD0: fprintf(pDUMP,"ìÅ"); break;
					case 0xD1: fprintf(pDUMP,"âŒ"); break;
					case 0xD2: fprintf(pDUMP,"âä"); break;
					case 0xD3: fprintf(pDUMP,"ó≥"); break;
					case 0xD4: fprintf(pDUMP,"ìV"); break;
					case 0xD5: fprintf(pDUMP,"â©"); break;
					case 0xD6: fprintf(pDUMP,"ãπ"); break;
					case 0xD7: fprintf(pDUMP,"ég"); break;
					case 0xD8: fprintf(pDUMP,"à≈"); break;
					case 0xD9: fprintf(pDUMP,"ìπ"); break;
					case 0xDA: fprintf(pDUMP,"íÖ"); break;
					case 0xDB: fprintf(pDUMP,"äv"); break;
					case 0xDC: fprintf(pDUMP,"â§"); break;
					case 0xDD: fprintf(pDUMP,"ä•"); break;
					case 0xDE: fprintf(pDUMP,"óÏ"); break;
					case 0xDF: fprintf(pDUMP,"âH"); break;
					case 0xE0: fprintf(pDUMP,"ïÍ"); break;
					case 0xE1: fprintf(pDUMP,"î≤"); break;
					case 0xE2: fprintf(pDUMP,"èo"); break;
					case 0xE3: fprintf(pDUMP,"ë∫"); break;
					case 0xE4: fprintf(pDUMP,"éq"); break;
					case 0xE5: fprintf(pDUMP,"ñC"); break;
					case 0xE6: fprintf(pDUMP,"îö"); break;
					case 0xE7: fprintf(pDUMP,"íe"); break;
					case 0xE8: fprintf(pDUMP,"êj"); break;
					case 0xE9: fprintf(pDUMP,"ît"); break;
					case 0xEA: fprintf(pDUMP,"ñÚ"); break;
					case 0xEB: fprintf(pDUMP,"ïó"); break;
					case 0xEC: fprintf(pDUMP,"ëæ"); break;
					case 0xED: fprintf(pDUMP,"å€"); break;
					case 0xEE: fprintf(pDUMP,"éÌ"); break;
					case 0xEF: fprintf(pDUMP,"î†"); break;
					case 0xF0: fprintf(pDUMP,"ñ°"); break;
					case 0xF1: fprintf(pDUMP,"ó†"); break;
					case 0xF2: fprintf(pDUMP,"ä€"); break;
					case 0xF3: fprintf(pDUMP,"åé"); break;
					case 0xF4: fprintf(pDUMP,"ó÷"); break;
					case 0xF5: fprintf(pDUMP,"ã‚"); break;
					case 0xF6: fprintf(pDUMP,"ñÓ"); break;
					case 0xF7: fprintf(pDUMP,"ã|"); break;
					case 0xF8: fprintf(pDUMP,"ìS"); break;
					case 0xF9: fprintf(pDUMP,"î\\"); break;//more wierdness
					case 0xFA: fprintf(pDUMP,"ãê"); break;
					case 0xFB: fprintf(pDUMP,"ód"); break;
					case 0xFC: fprintf(pDUMP,"èC"); break;
					case 0xFD: fprintf(pDUMP,"ïï"); break;
					case 0xFE: fprintf(pDUMP,"àÛ"); break;
					case 0xFF: fprintf(pDUMP,"ï∑"); break;
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
