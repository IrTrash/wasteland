#pragma once

const unsigned object_dmax = 256;
class object
{
	public :
	
	
		
		object() : dnum(0),dmax(object_dmax),type(0)
		{
			this->dlist = new data[this->dmax];
		}
		
		object(_type t) : dnum(0),dmax(object_dmax),type(t)
		{
		}
		
		~object()
		{
			delete[] this->dlist;
		}
		
		bool adddata(_type dtype,value *v,unsigned vnum)
		{
			this->sort();
			
			if(dtype == 0 || (v == NULL && vnum == 0) || this->dnum >= this->dmax )
			{
				return false;
			}
			
			data *buf = &this->dlist[this->dnum];
			
			buf->type = dtype;
			buf->vnum = vnum;
			for(unsigned n = 0; n < vnum; n++)
			{
				buf->add(&v[n]);
			}
			return true;
		}
		
		bool adddata(data *dest)
		{
			this->sort();
			
			if(dest == NULL || this->dnum >= this->dmax)
			{
				return false;
			}
			else if(dest->type == 0)
			{
				return false;
			}
			
			data *buf = &this->dlist[this->dnum];
			
			return buf->copy(dest);
		}
		
		bool deldatafirst(_type desttype)
		{
			if(desttype == 0)
			{
				return false;
			}
			
			for(unsigned n=0;n<this->dnum;n++)
			{
				if(this->dlist[n].type == desttype)
				{
					data nulldata;
					this->dlist[n].copy(&nulldata);
					this->sort();
					return true;
				}
			}
			return false;
		}
		
		data *getdfirst(_type desttype)
		{
			if(desttype == 0)
			{
				return NULL;
			}
			
			this->sort();
			for(unsigned n=0;n<this->dnum;n++)
			{
				if(this->dlist[n].type == desttype )
				{
					return &this->dlist[n];
				}
			}
			
			return NULL;
		}
		
		data *getdbyindex(unsigned index)
		{
			if(index >= this->dnum)
			{
				return NULL;
			}
			return &this->dlist[index];
		}
		
		bool copy(object* dest)
		{
			if(dest == NULL)
			{
				return false;
			}
			
			this->type = dest->type;
			this->dnum = dest->dnum;
			for(unsigned n=0;n<this->dnum;n++)
			{
				this->dlist[n].copy(dest->getdbyindex(n));
			}
			
			return true;
		}
		
		
	private :	
		_type type;
		unsigned dnum,dmax;
		data *dlist;
		
		
		void sort()
		{
			int i = 0;
			for(unsigned n=0;n<this->dnum;n++)
			{
				if(this->dlist[n].type != 0)
				{
					this->dlist[i++].copy(&this->dlist[n]);
				}
			}
			this->dnum = i;
			
			data nulldata;
			for(unsigned n = i;n<this->dmax;n++)
			{
				this->dlist[n].copy(&nulldata);
			}
		}
};
