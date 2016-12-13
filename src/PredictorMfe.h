
#ifndef PREDICTORMFE_H_
#define PREDICTORMFE_H_


#include "Predictor.h"

#include <list>

/**
 * Generic Predictor interface for MFE interaction computation to avoid
 * code redundancy
 *
 * @author Martin Mann
 *
 */
class PredictorMfe : public Predictor {


public:

	/**
	 * Construction call the super constructor
	 */
	PredictorMfe( const InteractionEnergy & energy, OutputHandler & output );

	virtual ~PredictorMfe();

protected:


	//! access to the interaction energy handler of the super class
	using Predictor::energy;

	//! access to the output handler of the super class
	using Predictor::output;

	// TODO provide all data structures as arguments to make predict() call threadsafe

	//! list of interactions
	typedef std::list<Interaction> InteractionList;

	//! mfe interaction boundaries
	InteractionList mfeInteractions;

	//! minimal stacking energy
	const E_type minStackingEnergy;
	//! minimal interaction initiation energy
	const E_type minInitEnergy;
	//! minimal dangling end energy
	const E_type minDangleEnergy;
	//! minimal interaction end energy
	const E_type minEndEnergy;

	/**
	 * Initializes the global energy minimum storage
	 *
	 * @param reportMax the maximal number of (sub)optimal interactions to be
	 *            reported to the output handler
	 * @param reportNonOverlapping whether or not the reported interactions
	 *            should be non-overlapping or not
	 */
	virtual
	void
	initOptima( const size_t reportMax
				, const bool reportNonOverlapping );

	/**
	 * updates the global optimum to be the mfe interaction if needed
	 *
	 * @param i1 the index of the first sequence interacting with i2
	 * @param j1 the index of the first sequence interacting with j2
	 * @param i2 the index of the second sequence interacting with i1
	 * @param j2 the index of the second sequence interacting with j1
	 * @param hybridE the energy of the interaction only (init+loops)
	 */
	virtual
	void
	updateOptima( const size_t i1, const size_t j1
				, const size_t i2, const size_t j2
				, const E_type energy );


	/**
	 * Fills a given interaction (boundaries given) with the according
	 * hybridizing base pairs.
	 * Note, the
	 * @param interaction IN/OUT the interaction to fill
	 */
	virtual
	void
	traceBack( Interaction & interaction ) = 0;



	/**
	 * Calls for the stored mfe and suboptimal solutions traceBack(i)
	 * and pushes the according interactions to the output handler.
	 *
	 */
	virtual
	void
	reportOptima();

};

#endif /* PREDICTORMFE_H_ */