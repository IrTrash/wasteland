#pragma once



struct value
{
	_type type;
	vtype v;
};

value tvalue(_type type,vtype v)
{
	value r;
	r.type = type;
	r.v = v;
	return r;
}


const unsigned data_vmax = 256;
class data
{
	public :
		_type type;
		unsigned vmax,vnum;
		value *vlist;
	
	
	
	
		
	
	
		data() : type(0),vmax(data_vmax),vnum(0)
		{
			this->vlist = new value[this->vmax];
			for(int n=0;n<this->vmax;n++)
			{
				this->vlist[n].type = 0;
				this->vlist[n].v = 0;
			}
		}
		
		data(data *dest) : type(0),vmax(data_vmax),vnum(0)
		{
			this->vlist = new value[this->vmax];
			for(int n=0;n<this->vmax;n++)
			{
				this->vlist[n].type = 0;
				this->vlist[n].v = 0;
			}
			this->copy(dest);
		}
		
		~data()
		{
			delete[] this->vlist;
		}
		
		bool add(_type desttype, vtype destv)
		{
			this->sort();
			if(desttype == 0 || this->vnum >= this->vmax)
			{
				return false;
			}
			
			this->vlist[this->vnum].type = desttype;
			this->vlist[this->vnum++].v = destv;
			
			return true;
		}
		
		bool add(value *v)
		{
			if(v == NULL || this->vnum >= this->vmax)
			{
				return false;
			}
			
			this->vlist[this->vnum++] = *v;
		}
		
		unsigned add(value *v,unsigned vsize)
		{
			if(v == NULL || vsize == 0)
			{
				return 0;
			}
			
			unsigned r= 0;
			for(unsigned n=0;n<vsize;n++)
			{
				if(this->add(&v[n]))
				{
					r++;
				}
				else
				{
					return r;
				}
			}
			return r;
		}
		
		bool deletefirst(_type desttype)
		{
			if(desttype == 0)
			{
				return false;
			}
			
			for(unsigned n=0;n<this->vnum;n++)
			{
				if(this->vlist[n].type == desttype)
				{
					this->vlist[n].type = 0;
					this->vlist[n].v = 0;
					this->sort();
					return true;
				}
			}
			
			return false;
		}
		
		vtype getvfirst(_type desttype)
		{
			if(desttype == 0)
			{
				return 0;
			}
			
			for(unsigned n=0;n<this->vnum;n++)
			{
				if(this->vlist[n].type == desttype)
				{
					return this->vlist[n].v;
				}
			}
			return 0;
		}
		
		value *getfirst(_type desttype)
		{
			if(desttype == 0)
			{
				return NULL;
			}
			
			for(unsigned n=0;n<this->vnum;n++)
			{
				if(this->vlist[n].type == desttype)
				{
					return &this->vlist[n];
				}
			}
			
			return NULL;
		}
		
		bool setfirst(_type desttype, vtype destv)
		{
			if(desttype == 0)
			{
				return false;
			}
			
			this->sort();
			for(unsigned n=0;n<this->vnum;n++)
			{
				if(this->vlist[n].type == desttype)
				{
					this->vlist[n].v = destv;
					return true;
				}
			}
			
			return false;
		}
		
		bool setfirst(value *dest)
		{
			if(dest == NULL)
			{
				return false;
			}
			
			return this->setfirst(dest->type,dest->v);
		}
		
		bool operfirst(_type opertype, _type desttype, vtype destv)
		{
			if(desttype == 0)
			{
				return false;
			}
			
			
			value *vbuf = this->getfirst(desttype);
			if(vbuf == NULL)
			{
				return false;
			}
			
			using namespace mtypelist;
			switch(opertype)
			{
				case _enter :
				{
					vbuf->v = destv;
				}
				break;
				
				case _plus :
				{
					vbuf->v += destv;
				}
				break;
				
				case _minus :
				{
					vbuf->v -= destv;
				}
				break;
				
				case _multiple :
				{
					vbuf->v *= destv;
				}
				break;
				
				case _divide :
				{
					if(destv == 0)
					{
						return false;
					}
					vbuf->v /= destv;
				}
				break;
				
				case _add :
				{
					return this->add(desttype,destv);
				}
				break;
				
				case _delete :
				{
					return this->deletefirst(desttype);
				}
				break;
				
				default : return false;
			}
			
			return true;
		}
		
		bool operfirst(_type opertype,value *dest)
		{
			if(dest == NULL)
			{
				return false;
			}
			
			return this->operfirst(opertype,dest->type,dest->v);
		}
		
		bool exist(_type desttype)
		{
			if(desttype == 0)
			{
				return false;
			}
			
			for(unsigned n=0;n<this->vnum;n++)
			{
				if(this->vlist[n].type == desttype)
				{
					return true;
				}
			}
			
			return false;
		}
		
		unsigned count(_type desttype)
		{
			if(desttype == 0)
			{
				return 0;
			}
			
			unsigned r = 0;
			for(unsigned n=0;n<this->vnum;n++)
			{
				if(this->vlist[n].type == desttype)
				{
					r++;
				}
			}
			
			return r;
		}
		
		bool copy(data *dest)
		{
			if(dest == NULL)
			{
				return false;
			}
			
			this->type = dest->type;
			this->vnum = dest->vnum;
			this->vmax = dest->vmax;
			
			for(unsigned n=0;n<this->vmax;n++)
			{
				this->vlist[n] = dest->vlist[n];
			}
			
			
			return true;
		}

	private :
		
		void sort()
		{
			int i = 0;
			for(unsigned n=0;n<this->vmax;n++)
			{
				if(this->vlist[n].type != 0 )
				{
					this->vlist[i++] = this->vlist[n];
				}
			}
			this->vnum = i;
			for(unsigned n = i;n<this->vmax;n++)
			{
				this->vlist[n].type = 0;
				this->vlist[n].v = 0;
			}
		}
};
