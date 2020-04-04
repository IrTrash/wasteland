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



class paintplan
{
	public :
	
		paintplan() : fnum(0),fmax(1024)
		{
			this->flist = new figure*[this->fmax];
			for(unsigned n=0;n<this->fmax;n++)
			{
				this->flist[n] = NULL;
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
			for(unsigned n=r;n<this->fmax;n++)
			{
				this->flist[n] = NULL;
			}
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
		
	
	
	
		void paint(HDC dc)
		{
			for(unsigned n=0;n<this->fnum;n++)
			{
				this->flist[n]->paint(dc);
			}
		}
		
		
	figure **flist;
	unsigned fnum,fmax;	
};
