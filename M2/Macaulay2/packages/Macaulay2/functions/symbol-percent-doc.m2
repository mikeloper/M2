--- status: DRAFT
--- author(s): kummini, MES
--- notes: 

document { 
     Key => {
	  "methods for normal forms and remainder",
	  (symbol %, RingElement, Ideal),
	  (symbol %, RingElement, MonomialIdeal),
	  (symbol %, RingElement, Matrix),
	  (symbol %, RingElement, RingElement),
	  (symbol %, Matrix, Ideal),
	  (symbol %, Matrix, MonomialIdeal),
	  (symbol %, Matrix, RingElement),
	  (symbol %, Matrix, Module),
	  (symbol %, Matrix, Matrix)	  
	  },
     Headline => "calculate the normal form of ring elements and matrices",
     Usage => "f % I",
     Inputs => { "f" => Nothing => {ofClass RingElement, ", or ",
	       ofClass Matrix},
	     "I" => Nothing => {ofClass Ideal, ", ",
		  ofClass Matrix, ", or ",
		  ofClass RingElement}
	     },
     Outputs => {
	  Nothing => {"the normal form of ", TT "f", " with respect to a Groebner basis
	  of I"}
	  },
     "The result has the same type as ", TT "f", ".  The normal form of a matrix is
     a matrix of the same shape whose columns have been reduced to normal form by the
     Groebner basis of ", TT "I", ".",
     PARA{},
     "To reduce ", TT "f", " with respect to ", TT "I", 
     ", a (partial) Groebner basis of ", TT "I", " is computed, unless
     it has already been done, or unless ", TT "I", " is a ", TO MonomialIdeal, ".",
     EXAMPLE lines ///
	  R = ZZ/1277[x,y];
    	  I = ideal(x^3 - 2*x*y, x^2*y - 2*y^2 + x);
	  (x^3 - 2*x) % I
	  (x^3) % I
	  S = ZZ[x,y];
	  144*x^2*y^2 % (7*x*y-2)
     ///,
     PARA{},
     "If I is a matrix between free modules, then
     a Groebner basis of I is a Groebner basis of the submodule generated by the columns
     of the matrix.",
     EXAMPLE lines ///
     	  S = QQ[a..f]
     	  J = ideal(a*b*c-d*e*f,a*b*d-c*e*f, a*c*e-b*d*f)
	  C = res J
	  F = syz transpose C.dd_4
	  G = transpose C.dd_3
	  ///,
     "Since C is a complex, we know that the image of G is contained in the image of F.",
     EXAMPLE lines ///
	  F % G
	  G % F
	  Ext^3(coker gens J, S)
     ///,
     PARA{},
     "Normal forms work over quotient rings too.",
     EXAMPLE lines ///
     	  A = QQ[x,y,z]/(x^3-y^2-3)
	  I = ideal(x^4, y^4)
	  J = ideal(x^3*y^2, x^2*y^3)
	  (gens J) % I
     ///,
     PARA{},
     "Here is an example involving rational functions.",
     EXAMPLE lines ///
     	  kk = toField(frac(ZZ[a,b]))
	  B = kk[x,y,z]
	  I = ideal(a*x^2-b*x-y-1, 1/b*y^2-z-1)
	  gens gb I
	  x^2*y^2 % I
     ///,
     SeeAlso => {symbol "%", "Groebner bases", generators, toField,(symbol "%", Matrix, GroebnerBasis)},
     }

document { 
     Key => {
	  (symbol "%", Matrix, GroebnerBasis),
	  (symbol "%", RingElement, GroebnerBasis)
	  },
     Headline => "calculate the normal form of ring elements and 
          matrices using a (partially computed) Groebner basis",
     Usage => "f % G",
     Inputs => { "f" => Nothing => {ofClass RingElement, ", or ",
	       ofClass Matrix},
	     "G"
	     },
     Outputs => {
	  Nothing => {"the normal form of ", TT "f", " with respect 
	       to the partially computed Groebner basis ", TT "G"}
	  },
     "In the following example, the seventh power of the trace of the matrix M
     is in the ideal generated by the entries of the cube of M.  Since the
     ideal I is homogeneous, it is only required to compute the Groebner basis
     in degrees at most seven.",
     EXAMPLE lines ///
     	  R = QQ[a..i];
	  M = genericMatrix(R,a,3,3)
	  I = ideal(M^3);
     	  f = trace M
	  G = gb(I, DegreeLimit=>3)
     	  f^7 % G == 0
	  gb(I, DegreeLimit=>7)	  	  
	  f^7 % G
	  gb I
     ///,
     "In these homogeneous situations, Macaulay2 only computes the
     Groebner basis as far as required, as shown below.",
     EXAMPLE lines ///
	  I = ideal(M^3);
	  G = gb(I, StopBeforeComputation=>true)
	  f^7 % I
	  status G
     ///,
     SeeAlso => {"methods for normal forms and remainder", "Groebner bases", genericMatrix}
     }
