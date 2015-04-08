//---------------------------------------------------------------------------
//    $Id: filtered_matrix.h 23248 2011-01-23 06:03:57Z bangerth $
//    Version: $Name$
//
//    Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//---------------------------------------------------------------------------
#ifndef __deal2__constrained_matrix_h
#define __deal2__constrained_matrix_h



#include<deal.II/base/config.h>
#include<deal.II/base/smartpointer.h>
#include<deal.II/base/thread_management.h>
#include<deal.II/base/memory_consumption.h>
#include<deal.II/lac/pointer_matrix.h>
#include<deal.II/lac/constraint_matrix.h>
#include<deal.II/lac/vector_memory.h>
#include <vector>
#include <algorithm>

DEAL_II_NAMESPACE_OPEN

template <typename number> class Vector;
template <class VECTOR> class ConstrainedMatrixBlock;


/*! @addtogroup Matrix2
 *@{
 */


/**
 *
 * @author Luca Heltai 2011
 */

template<class VEC, class MATRIX> 
class ConstrainedOperator 
{
  public:
    ConstrainedOperator(const MATRIX &m,
			const ConstraintMatrix &c,
                        MPI_Comm comm = MPI_COMM_WORLD) :
		    constraints(c),
		    matrix(m),
                    mpi_communicator (MPI_COMM_WORLD),
                    n_mpi_processes (Utilities::MPI::n_mpi_processes(mpi_communicator)),
                    this_mpi_process (Utilities::MPI::this_mpi_process(mpi_communicator))
      {}
    
    
    
    void vmult(VEC& dst, const VEC &src) const;

    void distribute_rhs(VEC &rhs) const;
    
  private:
    const ConstraintMatrix &constraints;
    const MATRIX &matrix;
    MPI_Comm mpi_communicator;
    unsigned int n_mpi_processes;
    unsigned int this_mpi_process;
};

/*@}*/
/*---------------------- Inline functions -----------------------------------*/


//--------------------------------Iterators--------------------------------------//


template<class VEC, class MATRIX> 
void ConstrainedOperator<VEC,MATRIX>::vmult(VEC& dst, const VEC &src) const
{

  Vector<double> loc_src(src.size());
  loc_src = src;

  //std::cout<<"in vector "<<std::endl;
  // for (unsigned int i = 0; i < src.size(); i++)
  //     if (src.locally_owned_elements().is_element(i))
  //        std::cout<<i<<" ("<<this_mpi_process<<")  "<<loc_src(i)<<std::endl;

  matrix.vmult(dst, src);

          
  for(unsigned int i=0; i<dst.size(); ++i)
    if( (constraints.is_constrained(i)) &&
        (src.locally_owned_elements().is_element(i)) ) 
      {
	dst(i) = src(i);
	const std::vector< std::pair < unsigned int, double > >
	  * entries = constraints.get_constraint_entries (i);
	for(unsigned int j=0; j< entries->size(); ++j)
	  dst(i) -= (*entries)[j].second *
		    loc_src((*entries)[j].first);
      }

  //std::cout<<"out vector "<<std::endl;
  // for (unsigned int i = 0; i < dst.size(); i++)
  //     if (dst.locally_owned_elements().is_element(i))
  //        std::cout<<i<<" ("<<this_mpi_process<<")  "<<dst(i)<<std::endl;



}

template<class VEC, class MATRIX> 
void ConstrainedOperator<VEC,MATRIX>::distribute_rhs(VEC &rhs) const
{
  for(unsigned int i=0; i<rhs.size(); ++i)
    if( (constraints.is_constrained(i)) &&
        (rhs.locally_owned_elements().is_element(i)) )
      rhs(i) = constraints.get_inhomogeneity(i);
}


DEAL_II_NAMESPACE_CLOSE

#endif
/*----------------------------   filtered_matrix.h     ---------------------------*/