// Copyright 1995 Michael E. Stillman

#include "ring.hpp"
#include "monoid.hpp"
#include "monideal.hpp"
#include "respoly.hpp"
#include "polyring.hpp"

#include "freemod.hpp"

void Ring::initialize_ring(int P0,
			   int nvars,
			   int totalvars,
			   const Monoid *D)
{
  // Remember: if this is a poly ring, the ring is K[M].
  // If this is a basic routine, K = this, M = trivial monoid.
  // If this is a frac field, K = R, M = trivial monoid.
  P = P0;
  _nvars = nvars;
  _totalvars = totalvars;
  D_ = D;

  if (D_->n_vars() > 0)
    {
      _HRing = PolynomialRing::create(globalZZ, D_);
    }
  else
    _HRing = 0;

  _zero_divisor = ZERO_RINGELEM;
  _isfield = false;

  zeroV = ZERO_RINGELEM;
  oneV = ZERO_RINGELEM;
  minus_oneV = ZERO_RINGELEM;
}

Ring::~Ring()
{
}

FreeModule *Ring::make_FreeModule() const
{ 
  return new FreeModule(this); 
}

FreeModule *Ring::make_FreeModule(int n) const
{ 
  return new FreeModule(this,n);
}

bool Ring::is_field() const 
{ 
  return _isfield; 
}
void Ring::declare_field() 
{ 
  _isfield = true; 
}
ring_elem Ring::get_zero_divisor() const 
{ 
  return copy(_zero_divisor); 
}


void Ring::remove_vector(vec &v) const
{
}

void Ring::mult_to(ring_elem &f, const ring_elem g) const
{
  f = mult(f,g);
}

void Ring::add_to(ring_elem &f, ring_elem &g) const
{
  f = add(f,g);
}

void Ring::subtract_to(ring_elem &f, ring_elem &g) const
{
  f = subtract(f,g);
}

void Ring::negate_to(ring_elem &f) const
{
  f = negate(f);
}

int Ring::coerce_to_int(ring_elem) const
{
  ERROR("cannot coerce given ring element to an integer");
  return 0;
}

ring_elem Ring::from_double(double a) const
{
  mpz_t f;
  mpz_init(f);
  mpz_set_d(f,a);
  ring_elem result = from_int(f);
  mpz_clear(f);
  return result;
}

ring_elem Ring::from_rational(mpq_ptr q) const
{
  return from_int(0);
}

ring_elem Ring::from_BigReal(mpf_ptr a) const
{
  return from_int(0);
}

ring_elem Ring::from_BigComplex(M2_BigComplex z) const
{
  return from_int(0);
}

ring_elem Ring::from_complex(M2_CC z) const
{
  return from_int(0);
}

ring_elem Ring::random() const
{
  ERROR("random scalar elements for this ring are not implemented");
  return 0;
}
ring_elem Ring::random(int /*homog*/, const int * /*deg*/) const
{
  ERROR("random non-scalar elements for this ring are not implemented");
  return 0;
}

ring_elem Ring::preferred_associate(ring_elem f) const
{
  // Here we assume that 'this' is a field:
  if (is_zero(f)) return from_int(1);
  return invert(f);
}

int Ring::n_terms(const ring_elem) const
{
  return 1;
}
ring_elem Ring::term(const ring_elem a, const int *) const
{
  return copy(a);
}
ring_elem Ring::lead_coeff(const ring_elem f) const
{
  return f;
}
ring_elem Ring::get_coeff(const ring_elem f, const int *) const
{
  return f;
}
ring_elem Ring::get_terms(const ring_elem f, int, int) const
{
  return f;
}

ring_elem Ring::diff(ring_elem a, ring_elem b, int use_coeff) const
{
  return mult(a,b);
}

bool Ring::in_subring(int n, const ring_elem a) const
{
  return true;
}

void Ring::degree_of_var(int n, const ring_elem a, int &lo, int &hi) const
{
  lo = 0;
  hi = 0;
}

ring_elem Ring::divide_by_var(int n, int d, const ring_elem a) const
{
  if (d == 0) return a;
  return from_int(0);
}

ring_elem Ring::divide_by_expvector(const int *exp, const ring_elem a) const
{
  return a;
}

ring_elem Ring::homogenize(const ring_elem f, int, int deg, const M2_arrayint) const
{
  if (deg != 0) 
    ERROR("homogenize: no homogenization exists");
  return f;
}

ring_elem Ring::homogenize(const ring_elem f, int, const M2_arrayint) const
{
  return f;
}

bool Ring::is_homogeneous(const ring_elem) const
{
  return true;
}

void Ring::degree(const ring_elem, int *d) const
{
  degree_monoid()->one(d);
}

bool Ring::multi_degree(const ring_elem f, int *d) const
  // returns true iff f is homogeneous
{
  degree_monoid()->one(d);
  return true;
}

void Ring::degree_weights(const ring_elem, const M2_arrayint, int &lo, int &hi) const
{
  lo = hi = 0;
}
int Ring::primary_degree(const ring_elem) const
{
  return 0;
}

int Ring::index_of_var(const ring_elem a) const
{
  return -1;
}

M2_arrayint Ring::support(const ring_elem a) const
{
  M2_arrayint result = makearrayint(0);
  return result;
}


////////////////////////////////////////////
// Translation gbvector <--> ringelem/vec //
////////////////////////////////////////////
// This version is valid ONLY for base rings (not QQ!)

#include "gbring.hpp"
Ring::trans_tag Ring::trans_type() const { return BASE; }

ring_elem Ring::trans_to_ringelem(ring_elem coeff, 
				  const int *exp) const
{
  return coeff;
}

ring_elem Ring::trans_to_ringelem_denom(ring_elem coeff, 
					ring_elem denom, 
					int *exp) const
{
  // To use this, the corresponding ring MUST have division defined
  return this->divide(coeff, denom);
}

void Ring::trans_from_ringelem(gbvectorHeap &H, 
				    ring_elem coeff, 
				    int comp, 
				    int *exp,
				    int firstvar) const
{
  GBRing *GR = H.get_gb_ring();
  const FreeModule *F = H.get_freemodule();
  const Monoid *M = GR->get_flattened_monoid();
  
  gbvector *g = GR->gbvector_term(F, coeff, comp);
  M->from_expvector(exp, g->monom);
  H.add(g);
}
////////////////////////////////////////////
/////////////////////////////////////////////////////


#include "text_io.hpp"
extern "C" void prelem(const Ring *R, const ring_elem f)
{
  buffer o;
  R->elem_text_out(o,f);
  emit(o.str());
}

// Local Variables:
// compile-command: "make -C $M2BUILDDIR/Macaulay2/e "
// End:
