/*
  Charmm output
*/

#include "iwconfig.h"
#include <stdlib.h>
#include <time.h>

#include "molecule.h"
#include "misc.h"

int
Molecule::write_molecule_crd(std::ostream & os)
{
  time_t current_time;
  (void) time(&current_time);
  int n_atoms = natoms();

  os<<"* CRD file generated by our own program (Eli Lilly and Company)\n";
  os<<"* Contact Jibo Wang (3-2190) or Michal Vieth (7-3959)\n";
  os<<"* Produced on "<<ctime(&current_time);
  os<<"*"<<'\n';
  // output atom type etc
  os<<n_atoms<<'\n';
  for (int i=0; i<n_atoms; i++)
    {
      IWString name = molecule_name();
      int name_length = name.length();

      if (name_length>3)
	;
      else
	{
	  for (int j=0; j<4-name_length; j++)
	    name.insert(' ', 0);
	  //	    output_name = output_name + " ";
	  //	  output_name = output_name + name;
	}

      name.to_uppercase();
      // atom number, "1", and molecule name
      os.width(5);
      os<<i+1;
      os.width(6);
      os<<"1 ";
      os.width(4);
      os<<"LIGA";
      // Per Dan's request, only output LIGA
      //      os<<name;

      // atom symbol and atom number

      /*      os.width(1);
      os<<" ";
      os<<atomic_symbol(i);
      os.setf(ios::left);
      os.width(4);
      os<<i+1;
      os.unsetf(ios::left);
      */

      os.width(1);
      os<<" ";
      os.setf(std::ios::left);
      IWString temp = atomic_symbol(i);
      temp.to_uppercase();
      temp.append_number(i+1);
      int temp_length = temp.length();
      if (temp_length>4)
	;
      else
	{
	  for (int j=0; j<5-temp_length; j++)
	    temp.append(1, " ");
	  //	    output_name = output_name + " ";
	  //	  output_name = output_name + name;
	}

      os<<temp;
      os.unsetf(std::ios::left);

      // charmm coordinates
      os.width(9);
      //      os.setf(std::ios::right);
      os.setf(std::ios::fixed);
      os.setf(std::ios::showpoint);
      os.precision(5);
      os<<x(i)<<" ";
      os.width(9);
      os<<y(i)<<" ";
      os.width(9);
      os<<z(i);
      os.unsetf(std::ios::fixed);
      os.unsetf(std::ios::showpoint);

      // molecule name
      os.width(1);
      os<<" ";
      os.width(4);
      os<<"LIGA";
      // Per Dan's request, only output LIGA
      //      os<<name;
      os.width(2);
      os<<" 1";

      // last field 0.0
      os.setf(std::ios::fixed);
      os.setf(std::ios::showpoint);
      os.precision(5);
      os.width(13);
      os<<0.0<<'\n';

    }
  return os.good ();
}

int
Molecule::write_molecule_psf (std::ostream & os)
{
  time_t current_time;
  (void) time(&current_time);
  int n_atoms = natoms();

  os<<"PSF"<<'\n'<<'\n';

  os.width(8);
  os<<"3"<<" !NTITILE"<<'\n';

  os<<"* PSF file generated by our own program (Eli Lilly and Company)\n";
  os<<"* Contact Jibo Wang (3-2190) or Michal Vieth (7-3959)\n";
  os<<"* Produced on "<<ctime(&current_time)<<'\n';
  
  // output atom type etc
  os.width(8);
  os<<n_atoms<<" !NATOM"<<'\n';
  for (int i=0; i<n_atoms; i++)
    {
      IWString name = molecule_name();
      int name_length = name.length();

      if (name_length>3)
	;
      else
	{
	  for (int j=0; j<4-name_length; j++)
	    name.insert(' ', 0);
	  //	    output_name = output_name + " ";
	  //	  output_name = output_name + name;
	}

      name.to_uppercase();
      // atom number and molecule name
      os.width(8);
      os<<i+1;
      os.width(1);
      os<<" ";
      os.width(4);
      os<<"LIGA";
      //per Dan's request, only ouput LIGA
      //os<<name;

      // always 1
      os<<" 1   ";

      // molecule name again
      os<<" LIGA";

      // per Dan's request, only output LIGA
      //      os<<" "<<name;
      
      // atom symbol and atom number
      os<<" ";
      os.setf(std::ios::left);
      IWString temp = atomic_symbol(i);
      temp.to_uppercase();
      temp.append_number(i+1);
      int temp_length = temp.length();
      if (temp_length>4)
	;
      else
	{
	  for (int j=0; j<5-temp_length; j++)
	    temp.append(1, " ");
	  //	    output_name = output_name + " ";
	  //	  output_name = output_name + name;
	}

      os<<temp;
      os.unsetf(std::ios::left);

      // charmm atom type
      os.width(4);
      //      os.setf(std::ios::right);
      os<<atom_type(i);

      charge_t q;

      // template charge
      if (NULL == _charges)
        q = static_cast<charge_t>(0.0);
      else
        q = _charges->item(i);

      os.width(15);
      os.precision(6);
      os.setf(std::ios::uppercase);
      os.setf(std::ios::scientific);
      os << q;
      os.unsetf(std::ios::scientific);
      os.unsetf(std::ios::uppercase);

      // atomic mass
      os.width(10);
      os.precision(4);
      os.setf(std::ios::fixed);
      os.setf(std::ios::showpoint);
      os<<atomic_mass(i);
      os.unsetf(std::ios::fixed);
      os.unsetf(std::ios::showpoint);

      os.width(12);
      os<<"0"<<'\n';
    }

  os<<'\n';

  // output bond list
  int n_bonds = nedges();
  os.width(8);
  os<<n_bonds;
  os<<" !NBONDS: bonds"<<'\n';

  int bond_pair = 0;
  for (int i=0; i<n_bonds; i++)
    {
      os.width(8);
      os<<bondi(i)->a1()+1;
      os.width(8);
      os<<bondi(i)->a2()+1;
      if (bond_pair++>2)
	{
	  bond_pair=0;
	  os<<'\n';
	}
    }
  if (0!=bond_pair) os<<'\n';

  int n_improper = 0;
  int * improper_center = new int [n_atoms];  // deleted at the end of the program

  set_vector(improper_center, n_atoms, 0);

  for (int i=0; i<n_atoms; i++)
    if ((6==atomic_number(i)) && (3==ncon(i)) && (4==nbonds(i)))
      {
	n_improper ++;
	improper_center [i] = 1;
      }
    else if ((7==atomic_number(i)) && (3==ncon(i)))
      {
	if (formal_charge(i)>0)
	  {
	    n_improper ++;
	    improper_center [i] = 1;
	  }
	else if (2 == hcount(i))
	  for (int j=0; j<3; j++)
	    {
	      int atomj = other(i, j);
	      if ((1!=atomic_number(atomj)) && (ncon(atomj) != nbonds(atomj)))
		{
		  n_improper ++;
		  improper_center [i] = 1;
		  break;
		}
	    }
      }

  // output empty angles and dihedrals
  os<<'\n';
  os.width(8);
  os<<0;
  os<<" !NTHETA: angles"<<'\n';
  os<<'\n'<<'\n';
  os.width(8);
  os<<0;
  os<<" !NPHI: dihedrals"<<'\n'<<'\n';

  // output improper list
  os<<'\n';
  os.width(8);
  os<<n_improper;
  os<<" !NIMPHI: impropers"<<'\n';

  int improper_pair = 0;
  for (int i=0; i<n_atoms; i++)
    if (1==improper_center [i])
      {
	os.width(8);
	os<<i+1;
	for (int j=0; j<3; j++)
	  {
	    os.width(8);
	    os<<other(i, j)+1;
	  }
	if (improper_pair++>0)
	  {
	    improper_pair=0;
	    os<<'\n';
	  }
      }

  delete [] improper_center;

  return os.good();
}

int
Molecule::write_molecule_wchm(std::ostream & os)
{
  time_t current_time;
  (void) time(&current_time);
  int n_atoms = natoms();

  os<<"! chm file generated by our own program (Eli Lilly and Company) to produce .chm file produced by weblab viewer\n";
  os<<"! Contact Jibo Wang (3-2190)\n";
  os<<"! Produced on "<<ctime(&current_time);
  os<<"! Same as Version 1.1\n";

  // output header
  os<<"QUERY NoName"<<'\n';
  os<<"{"<<'\n';
  os<<"  AND topology.root1 ( )"<<'\n';
  os<<"  {"<<'\n';
  os<<"    ATTRIBUTES :"<<'\n';
  os<<"      ROOT ;"<<'\n';
  os<<"    TOPOLOGY :"<<'\n';

  // define common used string
  IWString atom_string = "      ATOM ";
  IWString elements_string = "      ELEMENTS ";
  IWString coord2d_string = "      COORD2D ";
  IWString coord3d_string = "      COORD3D ";
  IWString bond_string = "      BOND ";

  IWString a0_symbol = "";
  IWString a1_symbol = "";

  for (int i=0; i<n_atoms; i++)
    {
      const Atom * atom_i = atomi(i);
      a0_symbol = atom_i->atomic_symbol();
      int n1 = i+1;
      os<<atom_string<<a0_symbol<<n1<<"."<<n1<<'\n';
      os<<elements_string<<"( "<<a0_symbol<<" )"<<'\n';
      os<<coord2d_string<<"( "<<atom_i->x()<<" "<<atom_i->y()<<" )"<<'\n';
      os<<coord3d_string<<"( "<<atom_i->x()<<" "<<atom_i->y()<<" "<<atom_i->z()<<" ) ;"<<'\n';
    }

  int nbond = nedges();
  for (int i=0;i<nbond;i++)
    {
      const Bond * bond_i = bondi(i);
      
      atom_number_t a0 = bond_i->a1();
      atom_number_t a1 = bond_i->a2();
      
      int n0 = a0+1;
      int n1 = a1+1;

      const Atom * atom_a0 = atomi(a0);
      const Atom * atom_a1 = atomi(a1);

      a0_symbol = atom_a0->atomic_symbol();
      a1_symbol = atom_a1->atomic_symbol();

      os<<bond_string<<a0_symbol<<n0<<n0<<"."<<a1_symbol<<n1<<n1;
      os<<" ( "<<a0_symbol<<n0<<"."<<n0<<" "<<a1_symbol<<n1<<"."<<n1<<" ) ";
      if (bond_i->is_single_bond())
	os<<"SINGLE ;"<<'\n';
      else if (bond_i->is_double_bond())
	os<<"DOUBLE ;"<<'\n';
      else if (bond_i->is_triple_bond())
	os<<"TRIPLE ;"<<'\n';
    }
  os<<"  }"<<'\n';
  os<<"}"<<'\n';
  return os.good();
}
