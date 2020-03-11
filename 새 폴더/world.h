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
			this->dlist = new data[this->dmax];
			this->objlist = new object[this->omax];
		}
		
		~world()
		{
			delete[] this->dlist;
			delete[] this->objlist;	
		}
	
	
		bool adddata(data *dest)
		{
			if(dest == NULL)
			{
				return false;
			}
			
			
			switch(dest->type)
			{
				
			}
		}
	
		bool setdata(_type objtype, _type objindex ,_type msgtype, vtype msgv)
		{
			
		}
		
		bool deldata(_type desttype)
		{
			
		};
		
	private :	
		data *dlist;
		unsigned dmax,dnum,dindex;
		
		object *objlist;
		unsigned omax,onum,oindex;
};
