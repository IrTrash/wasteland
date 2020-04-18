class figure //µµÇü 
{
	public :
		figure() : type(0),size(1),x(0),y(0)
		{
			this->brcolor = RGB(255,255,255);
		}
		
		figure(_type t,int dx, int dy, int s, COLORREF cr) : type(t),size(s),x(dx),y(dy),brcolor(cr)
		{
			
		}
		
		
		~figure()
		{
			
		}
		
		
		bool paint(HDC dc)
		{
			bool r = true;
			
			using namespace mtypelist;
			HBRUSH oldbr;
			oldbr = (HBRUSH)SelectObject(dc,CreateSolidBrush(this->brcolor));
			switch(this->type)
			{
				case _circle :
				{
					Ellipse(dc,this->x-this->size,this->y-this->size,this->x+this->size,this->y+this->size);
				}
				break;
				
				case _square :
				{
					Rectangle(dc,this->x-this->size,this->y-this->size,this->x+this->size,this->y+this->size);
				}
				break;
				
				
				default : r = false;
			}
			
			DeleteObject(SelectObject(dc,oldbr));
			return r;
		}
		
		
		_type type;
		int x,y,size;
		
		
		COLORREF pencolor,brcolor;
};

const unsigned textword_msglimit = 256;
class textword
{
	public :
	textword() : msglimit(textword_msglimit),msglen(0),csize(8),x(0),y(0)
	{
		this->msg = new char[this->msglimit];
		this->pencolor = RGB(0,0,0);
		this->bkcolor = RGB(255,255,255);
	}	
	
	textword(char *dest,int dx, int dy) : msglimit(textword_msglimit),msglen(0),csize(8),x(dx),y(dy)
	{
		this->msg = new char[this->msglimit];
		this->pencolor = RGB(0,0,0);
		this->bkcolor = RGB(255,255,255);
		strcpy(this->msg,dest);
		this->msglen = strlen(this->msg);
	}
	
	~textword()
	{
		delete[] this->msg;
	}
	
	bool setmsg(char *dest,unsigned destsize)
	{
		if(dest == NULL || destsize == 0)
		{
			return false;
		}
		
		if(strcpy(this->msg,dest) != NULL)
		{
			this->msglen = strlen(this->msg);
			return true;
		}
		
		
		this->msglen = 0;
		return false;
	}
	
	bool set(char *dest,int dx, int dy)
	{
		if(dest == NULL)
		{
			return false;
		}
		
		if(this->setmsg(dest,strlen(dest)))
		{
			this->x = dx;
			this->y = dy;
			return true;
		}
		
		return false;
	}
	
	bool paint(HDC dc)
	{
		
		HPEN oldpen = (HPEN)SelectObject(dc,CreatePen(PS_SOLID,1,pencolor));
		SetBkColor(dc,bkcolor);
		TextOutA(dc,x,y,msg,msglen);
		DeleteObject(SelectObject(dc,oldpen));
		
	}
	
	char *msg;
	int x,y;
	unsigned msglimit,msglen,csize;
	COLORREF pencolor,bkcolor;
};

class paintplan
{
	public :
	
		paintplan() : fnum(0),fmax(1024),tnum(0),tmax(1024)
		{
			this->flist = new figure*[this->fmax];
			for(unsigned n=0;n<this->fmax;n++)
			{
				this->flist[n] = NULL;
			}
			
			this->tlist = new textword*[this->tmax];
			for(unsigned n=0;n<this->tmax;n++)
			{
				this->tlist[n] = NULL;
			}
		}
	
	
		~paintplan()
		{
			for(unsigned n=0;n<this->fnum;n++)
			{
				if(this->flist[n] != NULL)
				{
					delete this->flist[n];
				}
			}
			delete[] flist;
			
			for(unsigned n=0;n<this->tnum;n++)
			{
				if(this->tlist[n] != NULL)
				{
					delete this->tlist[n];
				}
			}
			delete[] tlist;
		}
		
		void figuresort()
		{
			int r = 0;
			for(unsigned n=0;n<this->fnum;n++)
			{
				if(this->flist[n] != NULL)
				{
					this->flist[r++] = this->flist[n];
				}
			}
			for(unsigned n=r;n<this->fnum;n++)
			{
				this->flist[n] = NULL;
			}
			this->fnum = r;
		}
		
		bool addfigure(_type desttype,int destx, int desty, int destsize, COLORREF destcolor)
		{
			if(desttype == 0 || destsize == 0)
			{
				return false;
			}
			
			this->figuresort();
			if(this->fnum >= this->fmax)
			{
				return false;
			}
			
			this->flist[fnum++] = new figure(desttype,destx,desty,destsize,destcolor);
			return true;
		}
	
	
		bool addfigure(figure *dest)
		{
			if(dest == NULL)
			{
				return false;
			}
			return this->addfigure(dest->type,dest->x,dest->y,dest->size,dest->brcolor);
		}
		
	
		void textsort()
		{
			int r = 0;
			for(unsigned n=0;n<this->tnum;n++)
			{
				if(this->tlist[n] != NULL)
				{
					this->tlist[r++] = this->tlist[n];
				}
			}
			
			for(unsigned n=r;n<this->tnum;n++)
			{
				this->tlist[n] = NULL;
			}
			this->tnum = r;
		}
		
		bool addtext(char *dest,int dx, int dy)
		{
			if(dest == NULL || this->tnum >= this->tmax)
			{
				return false;
			}
			
			this->textsort();
			this->tlist[this->tnum++] = new textword(dest,dx,dy);
			return true;
		}
		
	
		void paint(HDC dc)
		{
			for(unsigned n=0;n<this->fnum;n++)
			{
				this->flist[n]->paint(dc);
			}
			
			for(unsigned n=0;n<this->tnum;n++)
			{
				this->tlist[n]->paint(dc);
			}
		}
		
		
	figure **flist;
	unsigned fnum,fmax;	
	textword **tlist;
	unsigned tnum,tmax;
};
