// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Material::An8Material(void)
{
	NbrAttribute = 0;

	LockAmbiantDiffuse = false;

	AmbiantIsTexture = false;
	AmbiantRGB[0]    = 224;
	AmbiantRGB[1]    = 224;
	AmbiantRGB[2]    = 224;
	AmbiantFactor    = 0.3f;
	AmbiantBlendMode = 0;
	AmbiantAlphaMode = 0;
	AmbiantTextureID = -1 ;
	
	DiffuseIsTexture = false;
	DiffuseRGB[0]    = 224;
	DiffuseRGB[1]    = 224;
	DiffuseRGB[2]    = 224;
	DiffuseFactor    = 0.7f;
	DiffuseBlendMode = 0;
	DiffuseAlphaMode = 0;
	DiffuseTextureID = -1 ;

	SpecularRGB[0]=255;
	SpecularRGB[1]=255;
	SpecularRGB[2]=255;
	SpecularFactor=0.200f;
	SpecularIsTexture=false;
	SpecularBlendMode=0;
	SpecularAlphaMode=0;
	SpecularTextureID = -1 ;

	EmissiveBlendMode=0;
	EmissiveAlphaMode=0;
	EmissiveRGB[0]=0;
	EmissiveRGB[1]=0;
	EmissiveRGB[2]=0;
	EmissiveFactor=0.000f;
	EmissiveIsTexture=false;
	EmissiveTextureID = -1 ;
			
	UseAlpha=false;	
	Alpha=255;
	Brilliance=1.0f;
	PhongSize=32;

	HasBumpMap = false; 
	HasTransparency = false;
	HasEnvironment = false;

	TransparencyPercent = 100;
	TransparencyBlendMode = AN8_DECAL;
	TransparencyAlphaMode = AN8_NONE;
	TransparencyTextureID = -1;
	
	EnvironmentPercent = 100;
	EnvironmentBlendMode = AN8_DECAL;
	EnvironmentAlphaMode = AN8_NONE;
	EnvironmentTextureID = -1;
	

	BumpMapPercent = 100;
	BumpMapBlendMode = AN8_DECAL;
	BumpMapAlphaMode = AN8_NONE;
	BumpMapTextureID = -1;



	//// Back /////
	HasBackSurface = false;

	BackLockAmbiantDiffuse = LockAmbiantDiffuse;

	BackAmbiantIsTexture = AmbiantIsTexture;
	BackAmbiantRGB[0]    = AmbiantRGB[0];
	BackAmbiantRGB[1]    = AmbiantRGB[1];
	BackAmbiantRGB[2]    = AmbiantRGB[2];
	BackAmbiantFactor    = AmbiantFactor;
	BackAmbiantBlendMode = AmbiantBlendMode;
	BackAmbiantAlphaMode = AmbiantAlphaMode;
	BackAmbiantTextureID = -1 ;
	
	BackDiffuseIsTexture = DiffuseIsTexture;
	BackDiffuseRGB[0]    = DiffuseRGB[0];
	BackDiffuseRGB[1]    = DiffuseRGB[1];
	BackDiffuseRGB[2]    = DiffuseRGB[2];
	BackDiffuseFactor    = DiffuseFactor;
	BackDiffuseBlendMode = DiffuseBlendMode;
	BackDiffuseAlphaMode = DiffuseAlphaMode;
	BackDiffuseTextureID = -1 ;

	BackSpecularIsTexture = SpecularIsTexture;
	BackSpecularRGB[0]    = SpecularRGB[0];
	BackSpecularRGB[1]    = SpecularRGB[1];
	BackSpecularRGB[2]    = SpecularRGB[2];
	BackSpecularFactor    = SpecularFactor;
	BackSpecularBlendMode = SpecularBlendMode;
	BackSpecularAlphaMode = SpecularAlphaMode;
	BackSpecularTextureID = -1 ;
	
	BackEmissiveRGB[0]    = EmissiveRGB[0];
	BackEmissiveRGB[1]    = EmissiveRGB[1];
	BackEmissiveRGB[2]    = EmissiveRGB[2];
	BackEmissiveFactor    = EmissiveFactor;
	BackEmissiveIsTexture = EmissiveIsTexture;
	BackEmissiveBlendMode = EmissiveBlendMode;
	BackEmissiveAlphaMode = EmissiveAlphaMode;
	BackEmissiveTextureID = -1 ;
	
	BackAlpha      = Alpha;
	BackBrilliance = Brilliance;
	BackPhongSize  = PhongSize;
		
	BackHasTransparency    = HasTransparency;   
	BackTransparencyPercent= TransparencyPercent;
	BackTransparencyBlendMode = AN8_DECAL;
	BackTransparencyAlphaMode = AN8_NONE;
	BackTransparencyTextureID = -1;
	
	BackHasEnvironment     = HasEnvironment;
	BackEnvironmentPercent = EnvironmentPercent;
	BackEnvironmentTextureID = EnvironmentTextureID ;
	BackEnvironmentBlendMode = AN8_DECAL;
	BackEnvironmentAlphaMode = AN8_NONE;
 	
	BackHasBumpMap     = HasBumpMap;   
	BackBumpMapPercent = BumpMapPercent;
	BackBumpMapTextureID = BumpMapTextureID ;
	BackBumpMapBlendMode = AN8_DECAL;
	BackBumpMapAlphaMode = AN8_NONE;	


	BackSpecularRGB[0]=255;
	BackSpecularRGB[1]=255;
	BackSpecularRGB[2]=255;
	BackSpecularFactor=0.200f;
	BackSpecularIsTexture=0;
	BackSpecularBlendMode=0;
	BackSpecularAlphaMode=0;

	BackEmissiveBlendMode=0;
	BackEmissiveAlphaMode=0;
	BackEmissiveRGB[0]=224;
	BackEmissiveRGB[1]=224;
	BackEmissiveRGB[2]=224;
	BackEmissiveFactor=0.000f;
	BackEmissiveIsTexture=0;
			
	BackUseAlpha=false;	
	BackAlpha=255;
	BackBrilliance=1.0f;
	BackPhongSize=32;

}

An8Material::~An8Material(void)
{

}


void An8Material::LoadSurface(AN8XPARSER*p_parser)
{
	int surfaceStart = p_parser->IndexOf(1,"surface {");
	int surfaceEnd = p_parser->IndexOf(1,"}");
	while(p_parser->Length()!=1+LINERETURN && surfaceStart != surfaceEnd)
	{
		//------------------AMBIANT--------------------------/////////////

		if(p_parser->FindWord("rgb { ")) // Old version of .an8 files
        {
			sscanf(p_parser->GetLine(),"      rgb { %d %d %d\n",&DiffuseRGB[0],
						&DiffuseRGB[1],
						&DiffuseRGB[2]
				);
        }	

		if(p_parser->FindWord("lockambiantdiffuse {"))
        {
			LockAmbiantDiffuse=true;
        }	
      
        if(p_parser->FindWord("ambiant {"))
        {			        
			////////////////RGB/////////////////////////////
			while(!p_parser->FindWord("rgb {"))
				p_parser->ReadLine();
					
			sscanf(p_parser->GetLine(),"		rgb { %d %d %d\n",&AmbiantRGB[0],
											  &AmbiantRGB[1],
											  &AmbiantRGB[2]
											  );
             									  
			////////////////FACTOR//////////////////////////
			while(!p_parser->FindWord("factor {"))
				p_parser->ReadLine();
			sscanf(p_parser->GetLine(),"		factor { %f\n",&AmbiantFactor);
		
			////////////////TEXTURENAME/////////////////////
			AmbiantIsTexture=false;
			p_parser->ReadLine();

			if(p_parser->FindWord("texturename {"))
			{
				AmbiantIsTexture=true;
				p_parser->GetName(&AmbiantTexName);
				// BlendMode /////
				p_parser->ReadLine();
	  			p_parser->ReadLine();
				if(p_parser->FindWord("blendmode { decal }"))
					AmbiantBlendMode=AN8_DECAL;
				else if(p_parser->FindWord("blendmode { darken }"))
					AmbiantBlendMode=AN8_DARKEN;
				else
					AmbiantBlendMode=AN8_LIGHTEN;
	                    
					// AlphaMode /////
					p_parser->ReadLine();
					if(p_parser->FindWord("alphamode { none }"))
						AmbiantAlphaMode=AN8_NONE;
					else if(p_parser->FindWord("alphamode { layer }"))
						AmbiantAlphaMode=AN8_LAYER;
					else
						AmbiantAlphaMode=AN8_FINAL;				    
			} // End of if(p_parser->FindWord("texturename {"))
		} // End of if(p_parser->FindWord("ambiant {"))

     		
			//------------------DIFFUSE---------------------////////////////////
			if(p_parser->FindWord("diffuse {") && !p_parser->FindWord("lockambiantdiffuse {"))	
			{			
				////////////////RGB/////////////////////////////
				while(!p_parser->FindWord("rgb {"))
					p_parser->ReadLine();
			
				sscanf(p_parser->GetLine(),"		rgb { %d %d %d\n",&DiffuseRGB[0],
						&DiffuseRGB[1],
						&DiffuseRGB[2]
				);

				////////////////FACTOR//////////////////////////
				while(!p_parser->FindWord("factor {"))
					p_parser->ReadLine();
				sscanf(p_parser->GetLine(),"		factor { %f\n",&DiffuseFactor);
				
				////////////////TEXTURENAME/////////////////////
				DiffuseIsTexture=false;
				p_parser->ReadLine();
				if(p_parser->FindWord("texturename {"))
				{
					DiffuseIsTexture=true;
					p_parser->GetName(&DiffuseTexName);
					// BlendMode /////
					p_parser->ReadLine();
					p_parser->ReadLine();
					if(p_parser->FindWord("blendmode { decal }"))
						DiffuseBlendMode=AN8_DECAL;
					else if(p_parser->FindWord("blendmode { darken }"))
						DiffuseBlendMode=AN8_DARKEN;
					else
						DiffuseBlendMode=AN8_LIGHTEN;
	                    
					// AlphaMode /////
					p_parser->ReadLine();
					if(p_parser->FindWord("alphamode { none }"))
						DiffuseAlphaMode=AN8_NONE;
					else if(p_parser->FindWord("alphamode { layer }"))
						DiffuseAlphaMode=AN8_LAYER;
					else
						DiffuseAlphaMode=AN8_FINAL;
				}  // End of if(p_parser->FindWord("texturename {")) 
			} // End of if(p_parser->FindWord("diffuse {"))	    
      	  
			//------------------SPECULAR----------------------//////////////////
			if(p_parser->FindWord("specular {"))
			{
				////////////////RGB/////////////////////////////
				while(!p_parser->FindWord("rgb {"))
					p_parser->ReadLine();
		
				sscanf(p_parser->GetLine(),"		rgb { %d %d %d\n",&SpecularRGB[0],
					&SpecularRGB[1],
					&SpecularRGB[2]
				);
				////////////////FACTOR//////////////////////////
				while(!p_parser->FindWord("factor {"))
					p_parser->ReadLine();
				sscanf(p_parser->GetLine(),"		factor { %f\n",&SpecularFactor);
				
				////////////////TEXTURENAME/////////////////////
				SpecularIsTexture=false;
				p_parser->ReadLine();
				if(p_parser->FindWord("texturename {"))
				{
					SpecularIsTexture=true;
					p_parser->GetName(&SpecularTexName);
					// BlendMode /////
					p_parser->ReadLine();
					p_parser->ReadLine();
					if(p_parser->FindWord("blendmode { decal }"))
						SpecularBlendMode=AN8_DECAL;
					else if(p_parser->FindWord("blendmode { darken }"))
						SpecularBlendMode=AN8_DARKEN;
					else
						SpecularBlendMode=AN8_LIGHTEN;
	                    
					// AlphaMode /////
					p_parser->ReadLine();
					if(p_parser->FindWord("alphamode { none }"))
						SpecularAlphaMode=AN8_NONE;
					else if(p_parser->FindWord("alphamode { layer }"))
						SpecularAlphaMode=AN8_LAYER;
					else
						SpecularAlphaMode=AN8_FINAL;           				    
				} 
			}// End Specular
	
			//------------------EMISSIVE----------------------//////////////////
			if(p_parser->FindWord("emissive {"))
			{
				////////////////RGB/////////////////////////////
				while(!p_parser->FindWord("rgb {"))
					p_parser->ReadLine();
		
				sscanf(p_parser->GetLine(),"		rgb { %d %d %d\n",&EmissiveRGB[0],
												  &EmissiveRGB[1],
												  &EmissiveRGB[2]
				);
	                  
				////////////////FACTOR//////////////////////////
				while(!p_parser->FindWord("factor {"))
					p_parser->ReadLine();
				sscanf(p_parser->GetLine(),"		factor { %f\n",&EmissiveFactor);
				
				////////////////TEXTURENAME/////////////////////
				EmissiveIsTexture=false;
				p_parser->ReadLine();
				if(p_parser->FindWord("texturename {"))
				{
					EmissiveIsTexture=true;
					p_parser->GetName(&EmissiveTexName);
					// BlendMode /////
					p_parser->ReadLine();
					p_parser->ReadLine();
					if(p_parser->FindWord("blendmode { decal }"))
						EmissiveBlendMode=AN8_DECAL;
					else if(p_parser->FindWord("blendmode { darken }"))
						EmissiveBlendMode=AN8_DARKEN;
					else
						EmissiveBlendMode=AN8_LIGHTEN;
	                    
					// AlphaMode /////
					p_parser->ReadLine();
					if(p_parser->FindWord("alphamode { none }"))
						EmissiveAlphaMode=AN8_NONE;
					else if(p_parser->FindWord("alphamode { layer }"))
						EmissiveAlphaMode=AN8_LAYER;
					else
						EmissiveAlphaMode=AN8_FINAL;
	           			
				}  			   
			}// End Emissive
			   		   
			//------------------ALPHA----------------------//////////////////			   
			if(p_parser->FindWord("alpha {"))
			{
				sscanf(p_parser->GetLine(),"      alpha { %d\n",&Alpha);
				UseAlpha = true;			   
			}
					   
			//------------------BRILLANCE----------------------//////////////////			   
			if(p_parser->FindWord("brilliance {"))
			{
				sscanf(p_parser->GetLine(),"      brilliance { %f\n",&Brilliance);			   
			}
	        	
			//------------------PHONGSIZE----------------------//////////////////
			if(p_parser->FindWord("phongsize {"))
			{	
				sscanf(p_parser->GetLine(),"      phongsize { %f\n",&PhongSize);			
			
				HasBumpMap=false;   
				BumpMapPercent=100;
				
				HasTransparency=false;   
				TransparencyPercent=100;
				
				HasEnvironment=false;   
				EnvironmentPercent=100;
			}


			if(p_parser->FindWord("map {"))
			{			         
				p_parser->ReadLine();
				//------------------MAP BUMP-------------------------///////////////
				if(p_parser->FindWord("kind { \"bumpmap\" }"))
				{                      
					HasBumpMap=true;  
					p_parser->ReadLine();// texture name
					p_parser->GetName(&BumpMapTexName);                       
					// BlendMode /////
					p_parser->ReadLine();
					p_parser->ReadLine();
					if(p_parser->FindWord("blendmode { decal }"))
						BumpMapBlendMode=AN8_DECAL;
					else if(p_parser->FindWord("blendmode { darken }"))
						BumpMapBlendMode=AN8_DARKEN;
					else
						BumpMapBlendMode=AN8_LIGHTEN;
	                    
					// AlphaMode /////
					p_parser->ReadLine();
					if(p_parser->FindWord("alphamode { none }"))
						BumpMapAlphaMode=AN8_NONE;
					else if(p_parser->FindWord("alphamode { layer }"))
						BumpMapAlphaMode=AN8_LAYER;
					else
						BumpMapAlphaMode=AN8_FINAL;
	                        
					p_parser->ReadLine();
					if(p_parser->FindWord("percent {"))
					{                           
						sscanf(p_parser->GetLine(),"		percent { %d\n",
							   &BumpMapPercent
						);                                                
					}                       
				}// End if (Kind bumpmap)	
				//------------------TRANSPARENCY-------------------------///////////////
				else if(p_parser->FindWord("kind { \"transparency\" }"))
				{                      
					HasTransparency=true;  
					p_parser->ReadLine();// texture name
					p_parser->GetName(&TransparencyTexName);                       
					// BlendMode /////
					p_parser->ReadLine();
					p_parser->ReadLine();
					if(p_parser->FindWord("blendmode { decal }"))
						TransparencyBlendMode = AN8_DECAL;
					else if(p_parser->FindWord("blendmode { darken }"))
						TransparencyBlendMode = AN8_DARKEN;
					else
						TransparencyBlendMode = AN8_LIGHTEN;
	                    
					// AlphaMode /////
					p_parser->ReadLine();
					if(p_parser->FindWord("alphamode { none }"))
						TransparencyAlphaMode = AN8_NONE;
					else if(p_parser->FindWord("alphamode { layer }"))
						TransparencyAlphaMode = AN8_LAYER;
					else
						TransparencyAlphaMode = AN8_FINAL;
	                        
					p_parser->ReadLine();
					if(p_parser->FindWord("percent {"))
					{                           
						sscanf(p_parser->GetLine(),"		percent { %d\n",
							   &TransparencyPercent
						);                                                
					}                        
				}// End if (Kind transparency)	
				//------------------ENVIRONEMENT-------------------------///////////////
				else if(p_parser->FindWord("kind { \"environment\" }"))
				{                      
					HasEnvironment=true;  
					p_parser->ReadLine();// texture name
					p_parser->GetName(&EnvironmentTexName);                       
					// BlendMode /////
					p_parser->ReadLine();
					p_parser->ReadLine();
					if(p_parser->FindWord("blendmode { decal }"))
						EnvironmentBlendMode = AN8_DECAL;
					else if(p_parser->FindWord("blendmode { darken }"))
						EnvironmentBlendMode = AN8_DARKEN;
					else
						EnvironmentBlendMode = AN8_LIGHTEN;
	                    
					// AlphaMode /////
					p_parser->ReadLine();
					if(p_parser->FindWord("alphamode { none }"))
						EnvironmentAlphaMode = AN8_NONE;
					else if(p_parser->FindWord("alphamode { layer }"))
						EnvironmentAlphaMode = AN8_LAYER;
					else
						EnvironmentAlphaMode = AN8_FINAL;
	                        
					p_parser->ReadLine();
					if(p_parser->FindWord("percent {"))
					{                           
						sscanf(p_parser->GetLine(),"		percent { %d\n",
							   &EnvironmentPercent
						);                                                
					}                        
				}// End if (Kind Environment)		         
			}// End if FindWord "map"
       	
			p_parser->ReadLine();
			surfaceEnd = p_parser->IndexOf(1,"}");
        
		} // End of we are not at the and of material
 
	
}

void An8Material::LoadBackSurface(AN8XPARSER*p_parser)
{
	int surfaceStart = p_parser->IndexOf(1,"backsurface {");
	int surfaceEnd = p_parser->IndexOf(1,"}");

	p_parser->ReadLine();

	while(p_parser->Length()!=1+LINERETURN && surfaceStart != surfaceEnd)
	{
		HasBackSurface = true;	

		if(p_parser->FindWord("rgb { ")) // Old version of .an8 files
        {
			sscanf(p_parser->GetLine(),"      rgb { %d %d %d\n",&BackDiffuseRGB[0],
						&BackDiffuseRGB[1],
						&BackDiffuseRGB[2]
				);
        }	
      
		if(p_parser->FindWord("lockambiantdiffuse {"))
		{
			BackLockAmbiantDiffuse=true;
		}
      
		if(p_parser->FindWord("ambiant {"))
		{				        
			////////////////RGB/////////////////////////////
			while(!p_parser->FindWord("rgb {"))
				p_parser->ReadLine();
					
			sscanf(p_parser->GetLine(),"		rgb { %d %d %d\n",&BackAmbiantRGB[0],
											&BackAmbiantRGB[1],
											 &BackAmbiantRGB[2]
											 );
             									  
			////////////////FACTOR//////////////////////////
			while(!p_parser->FindWord("factor {"))
				p_parser->ReadLine();
			sscanf(p_parser->GetLine(),"		factor { %f\n",&BackAmbiantFactor);
		
			////////////////TEXTURENAME/////////////////////
			BackAmbiantIsTexture=false;
			p_parser->ReadLine();
			if(p_parser->FindWord("texturename {"))
			{
				BackAmbiantIsTexture=true;
				p_parser->GetName(&BackAmbiantTexName);
				// BlendMode /////
				p_parser->ReadLine();
	  			p_parser->ReadLine();
				if(p_parser->FindWord("blendmode { decal }"))
					BackAmbiantBlendMode = AN8_DECAL;
				else if(p_parser->FindWord("blendmode { darken }"))
					BackAmbiantBlendMode = AN8_DARKEN;
				else
					BackAmbiantBlendMode = AN8_LIGHTEN;
                    
				// AlphaMode /////
				p_parser->ReadLine();
				if(p_parser->FindWord("alphamode { none }"))
					BackAmbiantAlphaMode=AN8_NONE;
				else if(p_parser->FindWord("alphamode { layer }"))
					BackAmbiantAlphaMode=AN8_LAYER;
				else
					BackAmbiantAlphaMode=AN8_FINAL;				    
			} // End of if(p_parser->FindWord("texturename {",oneline))
		} // End of if(p_parser->FindWord("ambiant {",oneline))  
        
		//------------------Diffuse--------------------------/////////////

		if(p_parser->FindWord("diffuse {") && !p_parser->FindWord("lockambiantdiffuse {"))	
		{			
			////////////////RGB/////////////////////////////
			while(!p_parser->FindWord("rgb {"))
				p_parser->ReadLine();
		
			sscanf(p_parser->GetLine(),"		rgb { %d %d %d\n",&BackDiffuseRGB[0],
										  &BackDiffuseRGB[1],
										  &BackDiffuseRGB[2]
										  );

			////////////////FACTOR//////////////////////////
			while(!p_parser->FindWord("factor {"))
				p_parser->ReadLine();
			sscanf(p_parser->GetLine(),"		factor { %f\n",&BackDiffuseFactor);
			
			////////////////TEXTURENAME/////////////////////
			BackDiffuseIsTexture=false;
			p_parser->ReadLine();
			if(p_parser->FindWord("texturename {"))
			{
				BackDiffuseIsTexture=true;
				p_parser->GetName(&BackDiffuseTexName);
				// BlendMode /////
				p_parser->ReadLine();
				p_parser->ReadLine();
				if(p_parser->FindWord("blendmode { decal }"))
					BackDiffuseBlendMode=AN8_DECAL;
				else if(p_parser->FindWord("blendmode { darken }"))
					BackDiffuseBlendMode=AN8_DARKEN;
				else
					BackDiffuseBlendMode=AN8_LIGHTEN;
                    
				// AlphaMode /////
				p_parser->ReadLine();
				if(p_parser->FindWord("alphamode { none }"))
					BackDiffuseAlphaMode=AN8_NONE;
				else if(p_parser->FindWord("alphamode { layer }"))
					BackDiffuseAlphaMode=AN8_LAYER;
				else
					BackDiffuseAlphaMode=AN8_FINAL;
			} // End of if Texturename
		} // End of Diffuse    
		//------------------SPECULAR----------------------//////////////////
		if(p_parser->FindWord("specular {"))
		{
			////////////////RGB/////////////////////////////
			while(!p_parser->FindWord("rgb {"))
				p_parser->ReadLine();
	
			sscanf(p_parser->GetLine(),"		rgb { %d %d %d\n",&BackSpecularRGB[0],
										  &BackSpecularRGB[1],
										  &BackSpecularRGB[2]
										  );
			////////////////FACTOR//////////////////////////
			while(!p_parser->FindWord("factor {"))
				p_parser->ReadLine();
			sscanf(p_parser->GetLine(),"		factor { %f\n",&BackSpecularFactor);
			
			////////////////TEXTURENAME/////////////////////
			BackSpecularIsTexture=false;
			p_parser->ReadLine();
			if(p_parser->FindWord("texturename {"))
			{
				BackSpecularIsTexture=true;
				p_parser->GetName(&BackSpecularTexName);
				// BlendMode /////
				p_parser->ReadLine();
				p_parser->ReadLine();
				if(p_parser->FindWord("blendmode { decal }"))
					BackSpecularBlendMode=AN8_DECAL;
				else if(p_parser->FindWord("blendmode { darken }"))
					BackSpecularBlendMode=AN8_DARKEN;
				else
					BackSpecularBlendMode=AN8_LIGHTEN;
                    
				// AlphaMode /////
				p_parser->ReadLine();
				if(p_parser->FindWord("alphamode { none }"))
					BackSpecularAlphaMode=AN8_NONE;
				else if(p_parser->FindWord("alphamode { layer }"))
					BackSpecularAlphaMode=AN8_LAYER;
				else
						BackSpecularAlphaMode=AN8_FINAL;           				    
			} 
		}// End Specular
			
		//------------------EMISSIVE----------------------//////////////////
		if(p_parser->FindWord("emissive {"))
		{
			////////////////RGB/////////////////////////////
			while(!p_parser->FindWord("rgb {"))
				p_parser->ReadLine();
	
			sscanf(p_parser->GetLine(),"		rgb { %d %d %d\n",&BackEmissiveRGB[0],
										  &BackEmissiveRGB[1],
										  &BackEmissiveRGB[2]
			);
                  
			////////////////FACTOR//////////////////////////
			while(!p_parser->FindWord("factor {"))
				p_parser->ReadLine();
			sscanf(p_parser->GetLine(),"		factor { %f\n",&BackEmissiveFactor);
			
			////////////////TEXTURENAME/////////////////////
			BackEmissiveIsTexture=false;
			p_parser->ReadLine();
			if(p_parser->FindWord("texturename {"))
			{
  				BackEmissiveIsTexture=true;
				p_parser->GetName(&BackEmissiveTexName);
				// BlendMode /////
				p_parser->ReadLine();
				p_parser->ReadLine();
				if(p_parser->FindWord("blendmode { decal }"))
					BackEmissiveBlendMode=AN8_DECAL;
				else if(p_parser->FindWord("blendmode { darken }"))
					BackEmissiveBlendMode=AN8_DARKEN;
				else
					BackEmissiveBlendMode=AN8_LIGHTEN;
                    
				// AlphaMode /////
				p_parser->ReadLine();
				if(p_parser->FindWord("alphamode { none }"))
					BackEmissiveAlphaMode=AN8_NONE;
				else if(p_parser->FindWord("alphamode { layer }"))
					BackEmissiveAlphaMode=AN8_LAYER;
				else
					BackEmissiveAlphaMode=AN8_FINAL;
           			
			}  			   
		}// End Emissive
			      
		//------------------ALPHA----------------------//////////////////			   
		if(p_parser->FindWord("alpha {"))
		{
			sscanf(p_parser->GetLine(),"      alpha { %d\n",&BackAlpha);
			BackUseAlpha = true;			   
		}
			   
		//------------------BRILLANCE----------------------//////////////////			   
		if(p_parser->FindWord("brilliance {"))
		{
			sscanf(p_parser->GetLine(),"      brilliance { %f\n",&BackBrilliance);			   
		}
		//------------------PHONGSIZE----------------------//////////////////
		if(p_parser->FindWord("phongsize {"))
		{	
			sscanf(p_parser->GetLine(),"      phongsize { %f\n",&BackPhongSize);			
		
			BackHasBumpMap=false;   
			BackBumpMapPercent=100;
		
			BackHasTransparency=false;   
			BackTransparencyPercent=100;
			
			BackHasEnvironment=false;   
			BackEnvironmentPercent=100;
		}
 
		if(p_parser->FindWord("map {"))
		{			         
			p_parser->ReadLine();
			//------------------MAP BUMP-------------------------///////////////
			if(p_parser->FindWord("kind { \"bumpmap\" }"))
			{                      
				BackHasBumpMap=true;  
				p_parser->ReadLine(); // texture name
				p_parser->GetName(&BackBumpMapTexName);                       
				// BlendMode /////
				p_parser->ReadLine();
				p_parser->ReadLine();
				if(p_parser->FindWord("blendmode { decal }"))
					BackBumpMapBlendMode=AN8_DECAL;
				else if(p_parser->FindWord("blendmode { darken }"))
					BackBumpMapBlendMode=AN8_DARKEN;
				else
					BackBumpMapBlendMode=AN8_LIGHTEN;
                    
				// AlphaMode /////
				p_parser->ReadLine();
				if(p_parser->FindWord("alphamode { none }"))
					BackBumpMapAlphaMode=AN8_NONE;
				else if(p_parser->FindWord("alphamode { layer }"))
					BackBumpMapAlphaMode=AN8_LAYER;
				else
					BackBumpMapAlphaMode=AN8_FINAL;
                        
				p_parser->ReadLine();
				if(p_parser->FindWord("percent {"))
				{                           
					sscanf(p_parser->GetLine(),"		percent { %d\n",
                          &BackBumpMapPercent
					);                                                
				}                        
			}// End if (Kind bumpmap)	
			//------------------TRANSPARENCY-------------------------///////////////
			else if(p_parser->FindWord("kind { \"transparency\" }"))
			{                      
				BackHasTransparency=true;  
				p_parser->ReadLine(); // texture name
				p_parser->GetName(&BackTransparencyTexName);                       
				// BlendMode /////
				p_parser->ReadLine();
				p_parser->ReadLine();
				if(p_parser->FindWord("blendmode { decal }"))
					BackTransparencyBlendMode=AN8_DECAL;
				else if(p_parser->FindWord("blendmode { darken }"))
					BackTransparencyBlendMode=AN8_DARKEN;
				else
					BackTransparencyBlendMode=AN8_LIGHTEN;
                    
				// AlphaMode /////
				p_parser->ReadLine();
				if(p_parser->FindWord("alphamode { none }"))
					BackTransparencyAlphaMode=AN8_NONE;
				else if(p_parser->FindWord("alphamode { layer }"))
					BackTransparencyAlphaMode=AN8_LAYER;
				else
					BackTransparencyAlphaMode=AN8_FINAL;
                        
				p_parser->ReadLine();
				if(p_parser->FindWord("percent {"))
				{                           
					sscanf(p_parser->GetLine(),"		percent { %d\n",
                          &BackTransparencyPercent
					);                                                
				}                        
			}// End if (Kind transparency)	
			//------------------ENVIRONEMENT-------------------------///////////////
			else if(p_parser->FindWord("kind { \"environment\" }"))
			{                      
				BackHasEnvironment=true;  
				p_parser->ReadLine(); // texture name
				p_parser->GetName(&BackEnvironmentTexName);                       
				// BlendMode /////
				p_parser->ReadLine();
				p_parser->ReadLine();
				if(p_parser->FindWord("blendmode { decal }"))
					BackEnvironmentBlendMode=AN8_DECAL;
				else if(p_parser->FindWord("blendmode { darken }"))
					BackEnvironmentBlendMode=AN8_DARKEN;
				else
					BackEnvironmentBlendMode=AN8_LIGHTEN;
                    
				// AlphaMode /////
				p_parser->ReadLine();
				if(p_parser->FindWord("alphamode { none }"))
					BackEnvironmentAlphaMode=AN8_NONE;
				else if(p_parser->FindWord("alphamode { layer }"))
					BackEnvironmentAlphaMode=AN8_LAYER;
				else
					BackEnvironmentAlphaMode=AN8_FINAL;
                        
				p_parser->ReadLine();
				if(p_parser->FindWord("percent {"))
				{                           
					sscanf(p_parser->GetLine(),"		percent { %d\n",
                          &BackEnvironmentPercent
					);                                                
					}                        
			}// End if (Kind Environment)		         
		}// End if FindWord "map"
		p_parser->ReadLine();
		surfaceEnd = p_parser->IndexOf(1,"}");
        
	} // End of while we are not at the and of backsurface
}

void An8Material::Load(AN8XPARSER*p_parser)
{

	int materialStart = p_parser->IndexOf(1,"material {");
	int materialEnd = p_parser->IndexOf(1,"}");

	p_parser->GetName(&Name);
	p_parser->ReadLine();

	
	while(p_parser->Length()!=1+LINERETURN && materialStart != materialEnd)
	{
		if(p_parser->FindWord("attribute {"))
		{
			++NbrAttribute;
			An8Attribute newAttribute;
			newAttribute.Load(p_parser);
			vAttributes.push_back(newAttribute);
		}
		if(p_parser->FindWord("surface {") && !p_parser->FindWord("backsurface {"))
		{
			LoadSurface(p_parser);
		}
		if(p_parser->FindWord("backsurface {"))
		{
			LoadBackSurface(p_parser);
		}
		p_parser->ReadLine();
		materialEnd = p_parser->IndexOf(1,"}");
	}
	p_parser->ReadLine();
}


void An8Material::SetDefault()
{
	//strcpy(Name," -- default --");
	Name = " -- default --";

	NbrAttribute = 0;

	LockAmbiantDiffuse=false;

	AmbiantIsTexture=false;
	AmbiantRGB[0]=224;
	AmbiantRGB[1]=224;
	AmbiantRGB[2]=224;
	AmbiantFactor=0.3f;
	AmbiantBlendMode=0;
	AmbiantAlphaMode=0;
	
	DiffuseIsTexture=false;
	DiffuseRGB[0]=224;
	DiffuseRGB[1]=224;
	DiffuseRGB[2]=224;
	DiffuseFactor=0.7f;
	DiffuseBlendMode=0;
	DiffuseAlphaMode=0;

	SpecularIsTexture=false;
	SpecularRGB[0]=255;
	SpecularRGB[1]=255;
	SpecularRGB[2]=255;
	SpecularFactor=0.2f;
	SpecularBlendMode=0;
	SpecularAlphaMode=0;
	
	EmissiveRGB[0]=224;
	EmissiveRGB[1]=224;
	EmissiveRGB[2]=224;
	EmissiveFactor=0.000f;
	EmissiveIsTexture=false;
	EmissiveBlendMode=0;
	EmissiveAlphaMode=0;
	
	Alpha=255;
	Brilliance=1;
	PhongSize=32;
		
	HasTransparency=false;   
	TransparencyPercent=100;
	
	HasEnvironment=false;
	EnvironmentPercent=100;
 	
	HasBumpMap=false;   
	BumpMapPercent=100;

	/// BACKSurface
	BackLockAmbiantDiffuse = LockAmbiantDiffuse;

	BackAmbiantIsTexture = AmbiantIsTexture;
	BackAmbiantRGB[0]    = AmbiantRGB[0];
	BackAmbiantRGB[1]    = AmbiantRGB[1];
	BackAmbiantRGB[2]    = AmbiantRGB[2];
	BackAmbiantFactor    = AmbiantFactor;
	BackAmbiantBlendMode = AmbiantBlendMode;
	BackAmbiantAlphaMode = AmbiantAlphaMode;
		
	BackDiffuseIsTexture = DiffuseIsTexture;
	BackDiffuseRGB[0]    = DiffuseRGB[0];
	BackDiffuseRGB[1]    = DiffuseRGB[1];
	BackDiffuseRGB[2]    = DiffuseRGB[2];
	BackDiffuseFactor    = DiffuseFactor;
	BackDiffuseBlendMode = DiffuseBlendMode;
	BackDiffuseAlphaMode = DiffuseAlphaMode;

  BackSpecularIsTexture = SpecularIsTexture;
  BackSpecularRGB[0]    = SpecularRGB[0];
  BackSpecularRGB[1]    = SpecularRGB[1];
  BackSpecularRGB[2]    = SpecularRGB[2];
  BackSpecularFactor    = SpecularFactor;
  BackSpecularBlendMode = SpecularBlendMode;
  BackSpecularAlphaMode = SpecularAlphaMode;
	
  BackEmissiveRGB[0]    = EmissiveRGB[0];
  BackEmissiveRGB[1]    = EmissiveRGB[1];
  BackEmissiveRGB[2]    = EmissiveRGB[2];
  BackEmissiveFactor    = EmissiveFactor;
  BackEmissiveIsTexture = EmissiveIsTexture;
  BackEmissiveBlendMode = EmissiveBlendMode;
  BackEmissiveAlphaMode = EmissiveAlphaMode;
	
  BackAlpha      = Alpha;
  BackBrilliance = Brilliance;
  BackPhongSize  = PhongSize;
		
  BackHasTransparency    = HasTransparency;   
  BackTransparencyPercent= TransparencyPercent;
	
  BackHasEnvironment     = HasEnvironment;
  BackEnvironmentPercent = EnvironmentPercent;
 	
  BackHasBumpMap     = HasBumpMap;   
  BackBumpMapPercent =BumpMapPercent;



}

