#pragma once

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
			
			using namespace mtypelist;
			
			
			data *d = this->dlist[destindex];
			if(d == NULL)
			{
				return 0;
			}
			unsigned r = 0;
			switch(opertype)
			{
				case _enter :
				{
					for(unsigned n=0;n<valuesize;n++)
					{
						if(d->setfirst(&destvalue[n]))
						{
							r++;
						}
						
					}
				}
				break;
				
				case _plus :
				{
					vtype v;
					for(unsigned n=0;n<valuesize;n++)
					{
						if(d->exist(destvalue[n].type))
						{
							d->setfirst(destvalue[n].type,d->getvfirst(destvalue[n].type) + destvalue[n].v);
							r++;
						}
						
					}
				}
				break;
				
				case _minus :
				{
					vtype v;
					for(unsigned n=0;n<valuesize;n++)
					{
						if(d->exist(destvalue[n].type))
						{
							d->setfirst(destvalue[n].type,d->getvfirst(destvalue[n].type) - destvalue[n].v);
							r++;
						}
						
					}
				}
				break;
				
				case _multiple :
				{
					vtype v;
					for(unsigned n=0;n<valuesize;n++)
					{
						if(d->exist(destvalue[n].type))
						{
							d->setfirst(destvalue[n].type,d->getvfirst(destvalue[n].type) * destvalue[n].v);
							r++;
						}
						
					}
				}
				break;
				
				case _divide :
				{
					vtype v;
					for(unsigned n=0;n<valuesize;n++)
					{
						if(d->exist(destvalue[n].type) && destvalue[n].v != 0)
						{
							d->setfirst(destvalue[n].type,d->getvfirst(destvalue[n].type) / destvalue[n].v);
							r++;
						}
						
					}
				}
				break;
				
				case _add :
				{
					return d->add(destvalue,valuesize);
				}
				break;
				
				case _delete :
				{
					for(unsigned n=0;n<valuesize;n++)
					{
						if(destvalue[n].type == __type)
						{
							d->deletefirst((_type)destvalue[n].v);
							r++;
						}
					}
				}
				break;
				
				
				default :
					return 0;
			}
			
			return r;
		}
		
		unsigned setdata_index(_type opertype, unsigned destindex, value *destvalue, unsigned valuesize,unsigned vindex)
		{
			
		}
		
		unsigned setobjdata(_type opertype, unsigned objindex, data *destdata, unsigned destsize)
		{
			
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
			
		}
		
		void pproc() //paint
		{
			
		}
};
