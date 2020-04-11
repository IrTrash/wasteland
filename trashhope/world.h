#pragma once
#include "math.h"
#define pi 3.141592

const unsigned _clock = 10;
const unsigned _fps = 60;


const unsigned world_objmax = 1024;
const unsigned world_datamax = 10000;

class world
{
	public :
		world() : dmax(world_datamax),dnum(0),dindex(0),omax(world_objmax),onum(0),oindex(0)
		{
			this->dlist = new data*[this->dmax];
			this->objlist = new object*[this->omax];
			
			for(unsigned n=0;n<this->dmax;n++)
			{
				this->dlist[n] = NULL;
			}
		
			this->boundcool = new unsigned*[this->omax];
			for(unsigned n=0;n<this->omax;n++)
			{
				this->objlist[n] = NULL;
				this->boundcool[n] = new unsigned[this->omax];
			}
				
		}
		
		~world()
		{
			for(unsigned n=0;n<this->dmax;n++)
			{
				if(this->dlist[n] != NULL)
				{
					delete this->dlist[n];
				}
			}
			delete[] this->dlist;
			
			for(unsigned n=0;n<this->omax;n++)
			{
				if(this->objlist[n] != NULL)
				{
					delete this->objlist[n];
				}
				delete[] this->boundcool[n];
 			}
			delete[] this->objlist;	
			delete[] this->boundcool;
		}
	
		
		void proc()
		{
			this->rproc();
		}
		
		
		void dsort()
		{
			unsigned r = 0;
			for(unsigned n=0;n<this->dnum;n++)
			{
				if(this->dlist[n] != NULL)
				{
					this->dlist[r++] = this->dlist[n];
				}
			}
			
			for(unsigned n=r;n<this->dnum;n++)
			{
				this->dlist[n] = NULL;
			}
			this->dnum = r;
			
		}
		
		void osort()
		{
			unsigned r = 0;
			for(unsigned n=0;n<this->onum;n++)
			{
				if(this->objlist[n] != NULL)
				{
					this->objlist[r++] = this->objlist[n];
				}
			}
			
			for(unsigned n=r;n<this->onum;n++)
			{
				this->dlist[n] = NULL;
			}
			this->onum = r;
		}
		
		bool adddata(data *dest)
		{
			this->dsort();
			if(dest == NULL || this->dnum >= this->dmax)
			{
				return false;
			}
			
			this->dlist[this->dnum] = new data;
			if(this->dlist[this->dnum]->copy(dest))
			{
				this->dnum++;
				return true;
			}
			else
			{
				delete this->dlist[this->dnum];
				return false;
			}
		}
		
		bool addobj(object *dest)
		{
			this->osort();
			if(dest == NULL || this->onum >= this->omax)
			{
				return false;
			}
			
			this->objlist[this->onum] = new object();
			if(this->objlist[this->onum]->copy(dest))
			{
				this->onum++;
				return true;
			}
			else
			{
				return false;
			}
		}
		
		
		bool deldata(unsigned index)
		{
			if(index >= this->dnum)
			{
				return false;
			}
			else if(this->dlist[index] == NULL)
			{
				return false;
			}
			
			delete this->dlist[index];
			this->dlist[index] = NULL;
			this->dsort();
			return true;
		}
		
		bool delobj(unsigned index)
		{
			if(index >= this->onum)
			{
				return false;
			}
			else if(this->objlist[index] == NULL)
			{
				return false;
			}
			
			delete this->objlist[index];
			this->objlist[index] = NULL;
			this->osort();
			return true;
		}
		
		unsigned setdata_first(_type opertype, unsigned destindex, value *destvalue, unsigned valuesize)
		{
			if(destindex >= this->dnum || destvalue == NULL || valuesize == 0)
			{
				return 0;
			}
			
			data *dbuf = this->dlist[destindex];
			if(dbuf == NULL)
			{
				return 0;
			}
			
			unsigned r = 0;
			for(unsigned n=0;n<valuesize;n++)
			{
				if(dbuf->operfirst(opertype,destvalue+n))
				{
					r++;
				}
			}
			return r;
		}
		
		unsigned setdata_index(_type opertype, unsigned destindex, value *destvalue, unsigned valuesize,unsigned vindex)
		{
			
		}
		
		unsigned setobjdata(_type opertype, unsigned objindex, data *destdata)
		{
			if(objindex >= this->onum || destdata == NULL )
			{
				return 0;
			}
			
			object *buf = this->objlist[objindex];
			if(buf == NULL)
			{
				return 0;
			}

			return buf->setdata(opertype,destdata);
		}
		
		
		
		data *getdatapfirst(_type desttype)
		{
			if(desttype == 0)
			{
				return NULL;
			}
			
			for(unsigned n=0;n<this->dnum;n++)
			{
				if(this->dlist[n]->type == desttype)
				{
					return this->dlist[n];
				}
			}
			return NULL;
		}
		
		
		paintplan pp;
		unsigned gridsize,width,height,gwidth,gheight;
	private :	
		data **dlist;
		unsigned dmax,dnum,dindex;
		
		object **objlist;
		unsigned omax,onum,oindex;
		
		_type type;
		unsigned **boundcool;
		
		
		void rproc()
		{
			this->dproc();
			this->oproc();
			this->inproc();
			
			
			this->pproc();
		}
		
		
		void dproc() //data
		{
			
		}
		
		
		void oproc() //object
		{
			using namespace mtypelist;
			using namespace dtypelist;
			using namespace otypelist;
			
			object *buf = NULL;
			data *stat = NULL, *pdata=NULL;;
			for(unsigned n=0;n<this->onum;n++)
			{
				if(this->objlist[n] == NULL)
				{
					continue;
				}
				
				buf = this->objlist[n];
				
				if(buf->type == _unit || buf->type == _bullet)
				{	
					pdata = buf->getdfirst(_pdata);
					if(stat != NULL)
					{
						
					}
					
					
					if(pdata != NULL)
					{
						_type movetype = (_type)pdata->getvfirst(_movetype);
						double x = pdata->getvfirst(_rx), y = pdata->getvfirst(_ry);
						switch(movetype)
						{
							case _destination :
							{								
								int dx = pdata->getvfirst(_destx),dy = pdata->getvfirst(_desty);
								int ix = (int)x, iy = (int)y;
								
								double distance = sqrt( (dx-ix)*(dx-ix) + (dy-iy)*(dy-iy) ) , direc;
								if(distance == 0) //??
								{
									break;
								}
								else 
								{
									direc = acos( double(dx-ix) / distance );
									if(iy > dy)
									{
										direc = -direc;
									}
								}
								pdata->setfirst(_direction,direc);
								pdata->setfirst(_movetype,_direction);
							} //이어지기 
							case _direction :
							{	
								double speed = pdata->getvfirst(_speed);
								if(speed == 0)
								{
									break;
								}		
								double direction = pdata->getvfirst(_direction);
								
								double rx = x + (speed*cos(direction)), ry = y + (speed*sin(direction));
								pdata->setfirst(_rx , rx );
								pdata->setfirst(_ry , ry );
								pdata->setfirst(_x , int(rx) );
								pdata->setfirst(_y , int(ry) );
							}
							break;
						}
						
						
					}
				}
				
				
				switch(buf->type)
				{
					case _bullet :
					{
						stat = buf->getdfirst(_status);
						
						
					}
					break;
					
					case _unit :
					{
						stat = buf->getdfirst(_status);
					}
					break;
				}
			}
		}
		
		void pproc() //paint
		{
			pp.fnum = 0;
			
			using namespace mtypelist;
			using namespace dtypelist;
			using namespace otypelist;
			
			for(unsigned n=0;n<this->onum;n++)
			{
				if(this->objlist[n] == NULL)
				{
					continue;
				}
				data *dbuf = objlist[n]->getdfirst(_pdata);
				if(dbuf == NULL)
				{
					continue;
				}
				
				switch(objlist[n]->type)
				{
					case _bullet :
					{		
						pp.addfigure(_circle,dbuf->getvfirst(_x),dbuf->getvfirst(_y),dbuf->getvfirst(_size),RGB(0,0,0));
					}
					break;
					
					case _unit :
					break;
				}
			}
		}
		
		
		
		public :
		
		bool iniobj(_type desttype, object *dest)
		{
			if(dest == NULL)
			{
				return false;
			}
			
			
			data dbuf;
			dest->type = desttype;
			dest->dnum = 0;
			
			using namespace mtypelist;
			using namespace dtypelist;
			using namespace otypelist;
			switch(desttype)
			{
				case _bullet :
				{
					dbuf.type = _pdata;
					dbuf.add(_x,0);
					dbuf.add(_y,0);
					dbuf.add(_rx,0);
					dbuf.add(_ry,0);
					dbuf.add(_destx,0);
					dbuf.add(_desty,0);
					dbuf.add(_size,1);
					dbuf.add(_speed,1);
					dbuf.add(_movetype,0);
					dbuf.add(_direction,0);
					dest->adddata(&dbuf);
					
					dbuf.reset();
					dbuf.type = _status; 
					dbuf.add(_life, 10);
					dbuf.add(_boundtype, _circle);
					dbuf.add(_bounddelay, 0);
					dbuf.add(_boundlimit, 1);
					dest->adddata(&dbuf);
				}
				break;
				
				case _unit:
				{
					dbuf.type = _pdata;
					dbuf.add(_x,0);
					dbuf.add(_y,0);
					dbuf.add(_rx,0);
					dbuf.add(_ry,0);
					dbuf.add(_destx,0);
					dbuf.add(_desty,0);
					dbuf.add(_size,1);
					dbuf.add(_speed,1);
					dbuf.add(_movetype,0);
					dbuf.add(_direction,0);
					dest->adddata(&dbuf);
					
					dbuf.reset();
					dbuf.type = _status; 
					dbuf.add(_life, 50);
					dbuf.add(_boundtype, _circle);
					dbuf.add(_bounddelay, 0);
					dbuf.add(_boundlimit, 1);
					dest->adddata(&dbuf);
				}
				break;
				
				default : return false;
			}
			
			return true;
		}
		
		
		void inproc() //상호작용 
		{
			using namespace mtypelist;
			using namespace dtypelist;
			using namespace otypelist;
			
			object *ubuf,*bbuf;
			for(unsigned n=0;n<this->onum;n++) //n : unit, m : bullet 
			{
				if(this->objlist[n] == NULL)
				{
					continue;
				}
				else if(this->objlist[n]->type != _unit)
				{
					continue;
				}
				
				ubuf = this->objlist[n];
				
				for(unsigned m=0;m<this->onum;m++)
				{
					if(n == m || this->objlist[m] == NULL)
					{
						continue;
					}
					else if(this->boundcool[n][m] > 0)
					{
						this->boundcool[n][m]--;
						continue;
					}
						
					bbuf = this->objlist[m];				
					switch(bbuf->type)
					{
						case _bullet :
						{
							data *updata = ubuf->getdfirst(_pdata), *bpdata = bbuf->getdfirst(_pdata);
							if(updata == NULL || bpdata == NULL)
							{
								break;
							}
							
							_type ubtype,bbtype,ubdelay,bbdelay,bblimit;
							ubtype = updata->getvfirst(_boundtype); 
							bbtype = bpdata->getvfirst(_boundtype);
							ubdelay = updata->getvfirst(_bounddelay); 
							bbdelay  = bpdata->getvfirst(_bounddelay);
							
							int ux,uy,bx,by,usize,bsize;
							ux = updata->getvfirst(_x);
							uy = updata->getvfirst(_y);
							bx = bpdata->getvfirst(_x);
							by = bpdata->getvfirst(_y);
							usize = updata->getvfirst(_size);
							bsize = bpdata->getvfirst(_size);
							
							if(ubtype == _circle && bbtype == _circle)
							{
								if( (usize+bsize)*(usize+bsize) >= (ux-bx)*(ux-bx) + (uy-by)*(uy-by) )
								{
									unsigned eftlistnum = bbuf->countdata(_beffect);
									data *eftlist = new data[eftlistnum];
									bbuf->getdata(_beffect,eftlist,eftlistnum);
									
									affectunit(ubuf,eftlist,eftlistnum);
									
									delete[] eftlist;
								}
							}
						}
						break;
						
						case _unit :
						break;
					}
				}
				
				
			}
		}
		
		void paint(HDC dc)
		{
			this->pp.paint(dc);
		}
		
		unsigned affectunit(object *destunit, data *dest, unsigned destsize)
		{
			if(destunit == NULL || dest == NULL || destsize == 0)
			{
				return 0;
			}
			
			using namespace mtypelist;
			using namespace dtypelist;
			
			_type etype = 0;
			for(unsigned n=0;n < destsize; n++)
			{
				etype = dest[n].getvfirst(__type);
				switch(etype)
				{
					case _dmg :
					{
					}
					break;
					
					case _heal :
					{
					}
					break;
				}
			}
		}
};
