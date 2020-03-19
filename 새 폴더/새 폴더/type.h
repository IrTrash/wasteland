const unsigned tnamelen = 32;

namespace mtypelist
{
	enum v
	{
		_x=1,_y
		
		
		
		,typelast
	};
	
	char name[typelast-1][tnamelen] =
	{
		"x","y"
	};
	
};

namespace dtypelist
{
	enum v
	{
		_posdata=1
		
		
		,typelast
	};
	
	char name[typelast-1][tnamelen] = 
	{
		"positiondata"
	};
	
};

namespace otypelist
{
	enum v
	{
		_bullet=1,_unit
		
		,typelast
	};
	
	char name[typelast-1][tnamelen] =
	{
		"bullet","unit"	
	};
};



#define _msg 1
#define _data 2
#define _obj 3
bool getname(_type c,_type desttype, char *dest)
{
	if(dest == NULL || desttype == 0)
	{
		return false;
	}
	
	switch(c)
	{
		case _msg :
		{
			using namespace mtypelist;
			if(desttype >= typelast)
			{
				return false;
			}
			strcpy(dest,name[desttype-1]);
		}
		break;
		
		case _data :
		{
			using namespace dtypelist;
			if(desttype >= typelast)
			{
				return false;
			}
			strcpy(dest,name[desttype-1]);
		}
		break;
		
		case _obj :
		{
			using namespace otypelist;
			if(desttype >= typelast)
			{
				return false;
			}
			strcpy(dest,name[desttype-1]);
		}
		break;
		
		default :
		return false;
		
	}
	
	
	return true;
}


_type gettype(_type c, char *dest)
{
	if(dest == NULL)
	{
		return false;
	}
	
	switch(c)
	{
		case _msg :
		{
			using namespace mtypelist;
			for(unsigned n = 1;n < typelast; n++)
			{
				if(strcmp(dest,name[n-1]) == 0)
				{
					return n;
				}
			}
		}		
		break;
		
		case _data :
		{
			using namespace dtypelist;
			for(unsigned n = 1;n < typelast; n++)
			{
				if(strcmp(dest,name[n-1]) == 0)
				{
					return n;
				}
			}
		}	
		break;
		
		case _obj :
		{
			using namespace otypelist;
			for(unsigned n = 1;n < typelast; n++)
			{
				if(strcmp(dest,name[n-1]) == 0)
				{
					return n;
				}
			}
		}	
		break;
	}
	
	
	return false;
}

