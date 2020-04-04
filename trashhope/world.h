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
			for(unsigned n=0;n<this->omax;n++)
			{
				this->objlist[n] = NULL;
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
			}
			delete[] this->objlist;	
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
		
		
		
		void rproc()
		{
			this->dproc();
			this->oproc();
			
			
			
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
				
				switch(buf->type)
				{
					case _bullet :
					{
						stat = buf->getdfirst(_status);
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
								} //�̾����� 
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
					break;
					
					case _unit :
					{
						stat = buf->getdfirst(_status);
						pdata = buf->getdfirst(_pdata);
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
					
				}
				break;
				
				case _unit:
				break;
				
				default : return false;
			}
			
			return true;
		}
		
		
		void paint(HDC dc)
		{
			this->pp.paint(dc);
		}
};
