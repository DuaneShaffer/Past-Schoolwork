#ifndef CUBER_H_INCLUDED
#define CUBER_H_INCLUDED


// class Cuber
// Class invariants: None.
class Cuber {
public:
	// operator()
	// Returns cube of its parameter.
	// Requirements on types: 
	//     Num must have op*, copy ctor.
	// Exception neutral.
	// Throws what & when Num operations throw.
	// Strong guarantee
	template<typename Num>
	Num operator()(const Num & k) const
	{
		return k * k * k;
	}
};



#endif // CUBER_H_INCLUDED