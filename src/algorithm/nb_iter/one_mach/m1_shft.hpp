/*
 * ******************** m1_nb_sh.hpp ******************************
 * 
 * description:      single machine SHIFT neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             15.12.1998
 *
 */

/*  class shift_Neighborhood
    This is the problem dependent SHIFT neighborhood class for the 
    single machine problem. This class is inherited from API_Neighborhood.

    author Andreas Winkler
    version 1.0a1
*/  

#ifndef __cplusplus
#error Must use C++ for these types.
#endif
#ifndef _m1_nb_sh_h
#define _m1_nb_sh_h

#include "../neighbor.hpp"
#include "../../../lisa_dt/schedule/m1_sched.hpp"
#include "../tabu.hpp"
#include "m1_api.hpp"

/** SHIFT-Neighborhood.
    This is the problem dependent SHIFT neighborhood class for the 
    single machine problem (i.e. this neighborhood swaps two arbitrary 
    jobs on the machine).

    @author Andreas Winkler
    @version 2.2a0
    @see Lisa_Neighborhood
    @see API_Neighborhood
*/
class shift_Neighborhood: public API_Neighborhood
  {
   private: int pos1, pos2, inc_dec;
   public:  
            /** construct shift_Neighborhood with a start schedule
	        and specified problem datas  */
	    shift_Neighborhood(Lisa_1Schedule*,Lisa_1Problem*);
	    /** propose a possible move,
	        parameter is ENUM or RAND for enumerative or random
	        generation of neighbor,\\
		it propose an interchange of two not necessary adjacent jobs
		on the machine,\\
		it returns OK or !OK */
	    int prepare_move( int );
	    /// do the proposed move \\it returns OK or !OK
	    int do_move();
  };

#endif



