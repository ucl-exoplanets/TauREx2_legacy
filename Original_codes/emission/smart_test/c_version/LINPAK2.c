/* LINPAK2.f -- translated by f2c (version 20100827).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Table of constant values */

static integer c__1 = 1;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* RCS version control information: */
/* $Header: LINPAK.f,v 2.1 2000/03/27 21:40:49 laszlo Exp $ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Call tree: */

/*    SGBCO */
/*       SASUM */
/*       SDOT */
/*       SAXPY */
/*       SGBFA */
/*           ISAMAX */
/*           SAXPY */
/*           SSCAL */
/*       SSCAL */
/*   SGBSL */
/*       SDOT */
/*       SAXPY */
/*   SGECO */
/*       SASUM */
/*       SDOT */
/*       SAXPY */
/*       SGEFA */
/*           ISAMAX */
/*           SAXPY */
/*           SSCAL */
/*       SSCAL */
/*   SGESL */
/*       SDOT */
/*       SAXPY */
/*   SSWAP */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Subroutine */ int sgbco_(real *abd, integer *lda, integer *n, integer *ml, 
	integer *mu, integer *ipvt, real *rcond, real *z__)
{
    /* System generated locals */
    integer abd_dim1, abd_offset, i__1, i__2, i__3, i__4;
    real r__1, r__2;

    /* Builtin functions */
    double r_sign(real *, real *);

    /* Local variables */
    static integer j, k, l, m;
    static real s, t;
    static integer kb, la;
    static real ek;
    static integer lm, mm, is, ju;
    static real sm, wk;
    static integer lz, kp1;
    static real wkm;
    static integer info;
    extern doublereal sdot_(integer *, real *, integer *, real *, integer *);
    extern /* Subroutine */ int sgbfa_(real *, integer *, integer *, integer *
	    , integer *, integer *, integer *), sscal_(integer *, real *, 
	    real *, integer *);
    static real anorm;
    extern doublereal sasum_(integer *, real *, integer *);
    static real ynorm;
    extern /* Subroutine */ int saxpy_(integer *, real *, real *, integer *, 
	    real *, integer *);

/*         Factors a real band matrix by Gaussian elimination */
/*         and estimates the condition of the matrix. */
/*         Revision date:  8/1/82 */
/*         Author:  Moler, C. B. (U. of New Mexico) */
/*     If  RCOND  is not needed, SGBFA is slightly faster. */
/*     To solve  A*X = B , follow SBGCO by SGBSL. */
/*     input: */
/*        ABD     REAL(LDA, N) */
/*                contains the matrix in band storage.  The columns */
/*                of the matrix are stored in the columns of  ABD  and */
/*                the diagonals of the matrix are stored in rows */
/*                ML+1 through 2*ML+MU+1 of  ABD . */
/*                See the comments below for details. */
/*        LDA     INTEGER */
/*                the leading dimension of the array  ABD . */
/*                LDA must be .GE. 2*ML + MU + 1 . */
/*        N       INTEGER */
/*                the order of the original matrix. */
/*        ML      INTEGER */
/*                number of diagonals below the main diagonal. */
/*                0 .LE. ML .LT. N . */
/*        MU      INTEGER */
/*                number of diagonals above the main diagonal. */
/*                0 .LE. MU .LT. N . */
/*                more efficient if  ML .LE. MU . */
/*     on return */
/*        ABD     an upper triangular matrix in band storage and */
/*                the multipliers which were used to obtain it. */
/*                The factorization can be written  A = L*U  where */
/*                L  is a product of permutation and unit lower */
/*                triangular matrices and  U  is upper triangular. */
/*        IPVT    INTEGER(N) */
/*                an integer vector of pivot indices. */
/*        RCOND   REAL */
/*                an estimate of the reciprocal condition of  A . */
/*                For the system  A*X = B , relative perturbations */
/*                in  A  and  B  of size  epsilon  may cause */
/*                relative perturbations in  X  of size  epsilon/RCOND . */
/*                If  RCOND  is so small that the logical expression */
/*                           1.0 + RCOND .EQ. 1.0 */
/*                is true, then  A  may be singular to working */
/*                precision.  In particular,  RCOND  is zero  if */
/*                exact singularity is detected or the estimate */
/*                underflows. */
/*        Z       REAL(N) */
/*                a work vector whose contents are usually unimportant. */
/*                If  A  is close to a singular matrix, then  Z  is */
/*                an approximate null vector in the sense that */
/*                norm(a*z) = rcond*norm(a)*norm(z) . */
/*     Band storage */
/*           If  A  is a band matrix, the following program segment */
/*           will set up the input. */
/*                   ML = (band width below the diagonal) */
/*                   MU = (band width above the diagonal) */
/*                   M = ML + MU + 1 */
/*                   DO 20 J = 1, N */
/*                      I1 = MAX(1, J-MU) */
/*                      I2 = MIN(N, J+ML) */
/*                      DO 10 I = I1, I2 */
/*                         K = I - J + M */
/*                         ABD(K,J) = A(I,J) */
/*                10    CONTINUE */
/*                20 CONTINUE */
/*           This uses rows  ML+1  through  2*ML+MU+1  of  ABD . */
/*           In addition, the first  ML  rows in  ABD  are used for */
/*           elements generated during the triangularization. */
/*           The total number of rows needed in  ABD  is  2*ML+MU+1 . */
/*           The  ML+MU by ML+MU  upper left triangle and the */
/*           ML by ML  lower right triangle are not referenced. */
/*     Example:  if the original matrix is */
/*           11 12 13  0  0  0 */
/*           21 22 23 24  0  0 */
/*            0 32 33 34 35  0 */
/*            0  0 43 44 45 46 */
/*            0  0  0 54 55 56 */
/*            0  0  0  0 65 66 */
/*      then  N = 6, ML = 1, MU = 2, LDA .GE. 5  and ABD should contain */
/*            *  *  *  +  +  +  , * = not used */
/*            *  * 13 24 35 46  , + = used for pivoting */
/*            * 12 23 34 45 56 */
/*           11 22 33 44 55 66 */
/*           21 32 43 54 65  * */
/* -------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*                       ** compute 1-norm of A */
    /* Parameter adjustments */
    abd_dim1 = *lda;
    abd_offset = 1 + abd_dim1;
    abd -= abd_offset;
    --ipvt;
    --z__;

    /* Function Body */
    anorm = 0.f;
    l = *ml + 1;
    is = l + *mu;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	r__1 = anorm, r__2 = sasum_(&l, &abd[is + j * abd_dim1], &c__1);
	anorm = dmax(r__1,r__2);
	if (is > *ml + 1) {
	    --is;
	}
	if (j <= *mu) {
	    ++l;
	}
	if (j >= *n - *ml) {
	    --l;
	}
/* L10: */
    }
/*                                               ** factor */
    sgbfa_(&abd[abd_offset], lda, n, ml, mu, &ipvt[1], &info);
/*     RCOND = 1/(norm(A)*(estimate of norm(inverse(A)))) . */
/*     estimate = norm(Z)/norm(Y) where  A*Z = Y  and  trans(A)*Y = E. */
/*     trans(A) is the transpose of A.  The components of E  are */
/*     chosen to cause maximum local growth in the elements of W  where */
/*     trans(U)*W = E.  The vectors are frequently rescaled to avoid */
/*     overflow. */
/*                     ** solve trans(U)*W = E */
    ek = 1.f;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	z__[j] = 0.f;
/* L20: */
    }
    m = *ml + *mu + 1;
    ju = 0;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	if (z__[k] != 0.f) {
	    r__1 = -z__[k];
	    ek = r_sign(&ek, &r__1);
	}
	if ((r__1 = ek - z__[k], dabs(r__1)) > (r__2 = abd[m + k * abd_dim1], 
		dabs(r__2))) {
	    s = (r__1 = abd[m + k * abd_dim1], dabs(r__1)) / (r__2 = ek - z__[
		    k], dabs(r__2));
	    sscal_(n, &s, &z__[1], &c__1);
	    ek = s * ek;
	}
	wk = ek - z__[k];
	wkm = -ek - z__[k];
	s = dabs(wk);
	sm = dabs(wkm);
	if (abd[m + k * abd_dim1] != 0.f) {
	    wk /= abd[m + k * abd_dim1];
	    wkm /= abd[m + k * abd_dim1];
	} else {
	    wk = 1.f;
	    wkm = 1.f;
	}
	kp1 = k + 1;
/* Computing MIN */
/* Computing MAX */
	i__3 = ju, i__4 = *mu + ipvt[k];
	i__2 = max(i__3,i__4);
	ju = min(i__2,*n);
	mm = m;
	if (kp1 <= ju) {
	    i__2 = ju;
	    for (j = kp1; j <= i__2; ++j) {
		--mm;
		sm += (r__1 = z__[j] + wkm * abd[mm + j * abd_dim1], dabs(
			r__1));
		z__[j] += wk * abd[mm + j * abd_dim1];
		s += (r__1 = z__[j], dabs(r__1));
/* L30: */
	    }
	    if (s < sm) {
		t = wkm - wk;
		wk = wkm;
		mm = m;
		i__2 = ju;
		for (j = kp1; j <= i__2; ++j) {
		    --mm;
		    z__[j] += t * abd[mm + j * abd_dim1];
/* L40: */
		}
	    }
	}
	z__[k] = wk;
/* L50: */
    }
    s = 1.f / sasum_(n, &z__[1], &c__1);
    sscal_(n, &s, &z__[1], &c__1);
/*                         ** solve trans(L)*Y = W */
    i__1 = *n;
    for (kb = 1; kb <= i__1; ++kb) {
	k = *n + 1 - kb;
/* Computing MIN */
	i__2 = *ml, i__3 = *n - k;
	lm = min(i__2,i__3);
	if (k < *n) {
	    z__[k] += sdot_(&lm, &abd[m + 1 + k * abd_dim1], &c__1, &z__[k + 
		    1], &c__1);
	}
	if ((r__1 = z__[k], dabs(r__1)) > 1.f) {
	    s = 1.f / (r__1 = z__[k], dabs(r__1));
	    sscal_(n, &s, &z__[1], &c__1);
	}
	l = ipvt[k];
	t = z__[l];
	z__[l] = z__[k];
	z__[k] = t;
/* L60: */
    }
    s = 1.f / sasum_(n, &z__[1], &c__1);
    sscal_(n, &s, &z__[1], &c__1);
    ynorm = 1.f;
/*                         ** solve L*V = Y */
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	l = ipvt[k];
	t = z__[l];
	z__[l] = z__[k];
	z__[k] = t;
/* Computing MIN */
	i__2 = *ml, i__3 = *n - k;
	lm = min(i__2,i__3);
	if (k < *n) {
	    saxpy_(&lm, &t, &abd[m + 1 + k * abd_dim1], &c__1, &z__[k + 1], &
		    c__1);
	}
	if ((r__1 = z__[k], dabs(r__1)) > 1.f) {
	    s = 1.f / (r__1 = z__[k], dabs(r__1));
	    sscal_(n, &s, &z__[1], &c__1);
	    ynorm = s * ynorm;
	}
/* L70: */
    }
    s = 1.f / sasum_(n, &z__[1], &c__1);
    sscal_(n, &s, &z__[1], &c__1);
    ynorm = s * ynorm;
/*                           ** solve  U*Z = W */
    i__1 = *n;
    for (kb = 1; kb <= i__1; ++kb) {
	k = *n + 1 - kb;
	if ((r__1 = z__[k], dabs(r__1)) > (r__2 = abd[m + k * abd_dim1], dabs(
		r__2))) {
	    s = (r__1 = abd[m + k * abd_dim1], dabs(r__1)) / (r__2 = z__[k], 
		    dabs(r__2));
	    sscal_(n, &s, &z__[1], &c__1);
	    ynorm = s * ynorm;
	}
	if (abd[m + k * abd_dim1] != 0.f) {
	    z__[k] /= abd[m + k * abd_dim1];
	}
	if (abd[m + k * abd_dim1] == 0.f) {
	    z__[k] = 1.f;
	}
	lm = min(k,m) - 1;
	la = m - lm;
	lz = k - lm;
	t = -z__[k];
	saxpy_(&lm, &t, &abd[la + k * abd_dim1], &c__1, &z__[lz], &c__1);
/* L80: */
    }
/*                              ** make znorm = 1.0 */
    s = 1.f / sasum_(n, &z__[1], &c__1);
    sscal_(n, &s, &z__[1], &c__1);
    ynorm = s * ynorm;
    if (anorm != 0.f) {
	*rcond = ynorm / anorm;
    }
    if (anorm == 0.f) {
	*rcond = 0.f;
    }
    return 0;
} /* sgbco_ */

/* Subroutine */ int sgbfa_(real *abd, integer *lda, integer *n, integer *ml, 
	integer *mu, integer *ipvt, integer *info)
{
    /* System generated locals */
    integer abd_dim1, abd_offset, i__1, i__2, i__3, i__4;

    /* Local variables */
    static integer i__, j, k, l, m;
    static real t;
    static integer i0, j0, j1, lm, mm, ju, jz, kp1, nm1;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *), 
	    saxpy_(integer *, real *, real *, integer *, real *, integer *);
    extern integer isamax_(integer *, real *, integer *);

/*         Factors a real band matrix by elimination. */
/*         Revision date:  8/1/82 */
/*         Author:  Moler, C. B. (U. of New Mexico) */
/*     SGBFA is usually called by SBGCO, but it can be called */
/*     directly with a saving in time if  RCOND  is not needed. */
/*     Input:  same as SGBCO */
/*     On return: */
/*        ABD,IPVT    same as SGBCO */
/*        INFO    INTEGER */
/*                = 0  normal value. */
/*                = k  if  u(k,k) .eq. 0.0 .  This is not an error */
/*                     condition for this subroutine, but it does */
/*                     indicate that SGBSL will divide by zero if */
/*                     called.  Use  RCOND  in SBGCO for a reliable */
/*                     indication of singularity. */
/*     (see SGBCO for description of band storage mode) */
/* ---------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    /* Parameter adjustments */
    abd_dim1 = *lda;
    abd_offset = 1 + abd_dim1;
    abd -= abd_offset;
    --ipvt;

    /* Function Body */
    m = *ml + *mu + 1;
    *info = 0;
/*                        ** zero initial fill-in columns */
    j0 = *mu + 2;
    j1 = min(*n,m) - 1;
    i__1 = j1;
    for (jz = j0; jz <= i__1; ++jz) {
	i0 = m + 1 - jz;
	i__2 = *ml;
	for (i__ = i0; i__ <= i__2; ++i__) {
	    abd[i__ + jz * abd_dim1] = 0.f;
/* L10: */
	}
/* L20: */
    }
    jz = j1;
    ju = 0;
/*                       ** Gaussian elimination with partial pivoting */
    nm1 = *n - 1;
    i__1 = nm1;
    for (k = 1; k <= i__1; ++k) {
	kp1 = k + 1;
/*                                  ** zero next fill-in column */
	++jz;
	if (jz <= *n) {
	    i__2 = *ml;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		abd[i__ + jz * abd_dim1] = 0.f;
/* L30: */
	    }
	}
/*                                  ** find L = pivot index */
/* Computing MIN */
	i__2 = *ml, i__3 = *n - k;
	lm = min(i__2,i__3);
	i__2 = lm + 1;
	l = isamax_(&i__2, &abd[m + k * abd_dim1], &c__1) + m - 1;
	ipvt[k] = l + k - m;
	if (abd[l + k * abd_dim1] == 0.f) {
/*                                      ** zero pivot implies this column */
/*                                      ** already triangularized */
	    *info = k;
	} else {
/*                                ** interchange if necessary */
	    if (l != m) {
		t = abd[l + k * abd_dim1];
		abd[l + k * abd_dim1] = abd[m + k * abd_dim1];
		abd[m + k * abd_dim1] = t;
	    }
/*                                      ** compute multipliers */
	    t = -1.f / abd[m + k * abd_dim1];
	    sscal_(&lm, &t, &abd[m + 1 + k * abd_dim1], &c__1);
/*                               ** row elimination with column indexing */
/* Computing MIN */
/* Computing MAX */
	    i__3 = ju, i__4 = *mu + ipvt[k];
	    i__2 = max(i__3,i__4);
	    ju = min(i__2,*n);
	    mm = m;
	    i__2 = ju;
	    for (j = kp1; j <= i__2; ++j) {
		--l;
		--mm;
		t = abd[l + j * abd_dim1];
		if (l != mm) {
		    abd[l + j * abd_dim1] = abd[mm + j * abd_dim1];
		    abd[mm + j * abd_dim1] = t;
		}
		saxpy_(&lm, &t, &abd[m + 1 + k * abd_dim1], &c__1, &abd[mm + 
			1 + j * abd_dim1], &c__1);
/* L40: */
	    }
	}
/* L50: */
    }
    ipvt[*n] = *n;
    if (abd[m + *n * abd_dim1] == 0.f) {
	*info = *n;
    }
    return 0;
} /* sgbfa_ */

/* Subroutine */ int sgbsl_(real *abd, integer *lda, integer *n, integer *ml, 
	integer *mu, integer *ipvt, real *b, integer *job)
{
    /* System generated locals */
    integer abd_dim1, abd_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer k, l, m;
    static real t;
    static integer kb, la, lb, lm, nm1;
    extern doublereal sdot_(integer *, real *, integer *, real *, integer *);
    extern /* Subroutine */ int saxpy_(integer *, real *, real *, integer *, 
	    real *, integer *);

/*         Solves the real band system */
/*            A * X = B  or  transpose(A) * X = B */
/*         using the factors computed by SBGCO or SGBFA. */
/*         Revision date:  8/1/82 */
/*         Author:  Moler, C. B. (U. of New Mexico) */
/*     Input: */
/*        ABD     REAL(LDA, N) */
/*                the output from SBGCO or SGBFA. */
/*        LDA     INTEGER */
/*                the leading dimension of the array  ABD . */
/*        N       INTEGER */
/*                the order of the original matrix. */
/*        ML      INTEGER */
/*                number of diagonals below the main diagonal. */
/*        MU      INTEGER */
/*                number of diagonals above the main diagonal. */
/*        IPVT    INTEGER(N) */
/*                the pivot vector from SBGCO or SGBFA. */
/*        B       REAL(N) */
/*                the right hand side vector. */
/*        JOB     INTEGER */
/*                = 0         to solve  A*X = B , */
/*                = nonzero   to solve  transpose(A)*X = B */
/*     On return */
/*        B       the solution vector  X */
/*     Error condition */
/*        A division by zero will occur if the input factor contains a */
/*        zero on the diagonal.  Technically, this indicates singularity, */
/*        but it is often caused by improper arguments or improper */
/*        setting of LDA .  It will not occur if the subroutines are */
/*        called correctly and if SBGCO has set RCOND .GT. 0.0 */
/*        or SGBFA has set INFO .EQ. 0 . */
/*     To compute  inverse(a) * c  where  c  is a matrix */
/*     with  p  columns */
/*           call sgbco(abd,lda,n,ml,mu,ipvt,rcond,z) */
/*           if (rcond is too small) go to ... */
/*           do 10 j = 1, p */
/*              call sgbsl(abd,lda,n,ml,mu,ipvt,c(1,j),0) */
/*        10 continue */
/* -------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    /* Parameter adjustments */
    abd_dim1 = *lda;
    abd_offset = 1 + abd_dim1;
    abd -= abd_offset;
    --ipvt;
    --b;

    /* Function Body */
    m = *mu + *ml + 1;
    nm1 = *n - 1;
    if (*job == 0) {
/*                           ** solve  A * X = B */
/*                               ** first solve L*Y = B */
	if (*ml != 0) {
	    i__1 = nm1;
	    for (k = 1; k <= i__1; ++k) {
/* Computing MIN */
		i__2 = *ml, i__3 = *n - k;
		lm = min(i__2,i__3);
		l = ipvt[k];
		t = b[l];
		if (l != k) {
		    b[l] = b[k];
		    b[k] = t;
		}
		saxpy_(&lm, &t, &abd[m + 1 + k * abd_dim1], &c__1, &b[k + 1], 
			&c__1);
/* L10: */
	    }
	}
/*                           ** now solve  U*X = Y */
	i__1 = *n;
	for (kb = 1; kb <= i__1; ++kb) {
	    k = *n + 1 - kb;
	    b[k] /= abd[m + k * abd_dim1];
	    lm = min(k,m) - 1;
	    la = m - lm;
	    lb = k - lm;
	    t = -b[k];
	    saxpy_(&lm, &t, &abd[la + k * abd_dim1], &c__1, &b[lb], &c__1);
/* L20: */
	}
    } else {
/*                          ** solve  trans(A) * X = B */
/*                                  ** first solve  trans(U)*Y = B */
	i__1 = *n;
	for (k = 1; k <= i__1; ++k) {
	    lm = min(k,m) - 1;
	    la = m - lm;
	    lb = k - lm;
	    t = sdot_(&lm, &abd[la + k * abd_dim1], &c__1, &b[lb], &c__1);
	    b[k] = (b[k] - t) / abd[m + k * abd_dim1];
/* L30: */
	}
/*                                  ** now solve trans(L)*X = Y */
	if (*ml != 0) {
	    i__1 = nm1;
	    for (kb = 1; kb <= i__1; ++kb) {
		k = *n - kb;
/* Computing MIN */
		i__2 = *ml, i__3 = *n - k;
		lm = min(i__2,i__3);
		b[k] += sdot_(&lm, &abd[m + 1 + k * abd_dim1], &c__1, &b[k + 
			1], &c__1);
		l = ipvt[k];
		if (l != k) {
		    t = b[l];
		    b[l] = b[k];
		    b[k] = t;
		}
/* L40: */
	    }
	}
    }
    return 0;
} /* sgbsl_ */

/* Subroutine */ int sgeco_(real *a, integer *lda, integer *n, integer *ipvt, 
	real *rcond, real *z__)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;
    real r__1, r__2;

    /* Builtin functions */
    double r_sign(real *, real *);

    /* Local variables */
    static integer j, k, l;
    static real s, t;
    static integer kb;
    static real ek, sm, wk;
    static integer kp1;
    static real wkm;
    static integer info;
    extern doublereal sdot_(integer *, real *, integer *, real *, integer *);
    extern /* Subroutine */ int sgefa_(real *, integer *, integer *, integer *
	    , integer *), sscal_(integer *, real *, real *, integer *);
    static real anorm;
    extern doublereal sasum_(integer *, real *, integer *);
    static real ynorm;
    extern /* Subroutine */ int saxpy_(integer *, real *, real *, integer *, 
	    real *, integer *);

/*         Factors a real matrix by Gaussian elimination */
/*         and estimates the condition of the matrix. */
/*         Revision date:  8/1/82 */
/*         Author:  Moler, C. B. (U. of New Mexico) */
/*         If  RCOND  is not needed, SGEFA is slightly faster. */
/*         To solve  A*X = B , follow SGECO by SGESL. */
/*     On entry */
/*        A       REAL(LDA, N) */
/*                the matrix to be factored. */
/*        LDA     INTEGER */
/*                the leading dimension of the array  A . */
/*        N       INTEGER */
/*                the order of the matrix  A . */
/*     On return */
/*        A       an upper triangular matrix and the multipliers */
/*                which were used to obtain it. */
/*                The factorization can be written  A = L*U , where */
/*                L  is a product of permutation and unit lower */
/*                triangular matrices and  U  is upper triangular. */
/*        IPVT    INTEGER(N) */
/*                an integer vector of pivot indices. */
/*        RCOND   REAL */
/*                an estimate of the reciprocal condition of  A . */
/*                For the system  A*X = B , relative perturbations */
/*                in  A  and  B  of size  epsilon  may cause */
/*                relative perturbations in  X  of size  epsilon/RCOND . */
/*                If  RCOND  is so small that the logical expression */
/*                           1.0 + RCOND .EQ. 1.0 */
/*                is true, then  A  may be singular to working */
/*                precision.  In particular,  RCOND  is zero  if */
/*                exact singularity is detected or the estimate */
/*                underflows. */
/*        Z       REAL(N) */
/*                a work vector whose contents are usually unimportant. */
/*                If  A  is close to a singular matrix, then  Z  is */
/*                an approximate null vector in the sense that */
/*                norm(A*Z) = RCOND*norm(A)*norm(Z) . */
/* ------------------------------------------------------------------ */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*                        ** compute 1-norm of A */
    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ipvt;
    --z__;

    /* Function Body */
    anorm = 0.f;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
/* Computing MAX */
	r__1 = anorm, r__2 = sasum_(n, &a[j * a_dim1 + 1], &c__1);
	anorm = dmax(r__1,r__2);
/* L10: */
    }
/*                                      ** factor */
    sgefa_(&a[a_offset], lda, n, &ipvt[1], &info);
/*     RCOND = 1/(norm(A)*(estimate of norm(inverse(A)))) . */
/*     estimate = norm(Z)/norm(Y) where  A*Z = Y  and  trans(A)*Y = E . */
/*     trans(A) is the transpose of A.  The components of E  are */
/*     chosen to cause maximum local growth in the elements of W  where */
/*     trans(U)*W = E.  The vectors are frequently rescaled to avoid */
/*     overflow. */
/*                        ** solve trans(U)*W = E */
    ek = 1.f;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	z__[j] = 0.f;
/* L20: */
    }
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	if (z__[k] != 0.f) {
	    r__1 = -z__[k];
	    ek = r_sign(&ek, &r__1);
	}
	if ((r__1 = ek - z__[k], dabs(r__1)) > (r__2 = a[k + k * a_dim1], 
		dabs(r__2))) {
	    s = (r__1 = a[k + k * a_dim1], dabs(r__1)) / (r__2 = ek - z__[k], 
		    dabs(r__2));
	    sscal_(n, &s, &z__[1], &c__1);
	    ek = s * ek;
	}
	wk = ek - z__[k];
	wkm = -ek - z__[k];
	s = dabs(wk);
	sm = dabs(wkm);
	if (a[k + k * a_dim1] != 0.f) {
	    wk /= a[k + k * a_dim1];
	    wkm /= a[k + k * a_dim1];
	} else {
	    wk = 1.f;
	    wkm = 1.f;
	}
	kp1 = k + 1;
	if (kp1 <= *n) {
	    i__2 = *n;
	    for (j = kp1; j <= i__2; ++j) {
		sm += (r__1 = z__[j] + wkm * a[k + j * a_dim1], dabs(r__1));
		z__[j] += wk * a[k + j * a_dim1];
		s += (r__1 = z__[j], dabs(r__1));
/* L30: */
	    }
	    if (s < sm) {
		t = wkm - wk;
		wk = wkm;
		i__2 = *n;
		for (j = kp1; j <= i__2; ++j) {
		    z__[j] += t * a[k + j * a_dim1];
/* L40: */
		}
	    }
	}
	z__[k] = wk;
/* L50: */
    }
    s = 1.f / sasum_(n, &z__[1], &c__1);
    sscal_(n, &s, &z__[1], &c__1);
/*                                ** solve trans(L)*Y = W */
    i__1 = *n;
    for (kb = 1; kb <= i__1; ++kb) {
	k = *n + 1 - kb;
	if (k < *n) {
	    i__2 = *n - k;
	    z__[k] += sdot_(&i__2, &a[k + 1 + k * a_dim1], &c__1, &z__[k + 1],
		     &c__1);
	}
	if ((r__1 = z__[k], dabs(r__1)) > 1.f) {
	    s = 1.f / (r__1 = z__[k], dabs(r__1));
	    sscal_(n, &s, &z__[1], &c__1);
	}
	l = ipvt[k];
	t = z__[l];
	z__[l] = z__[k];
	z__[k] = t;
/* L60: */
    }
    s = 1.f / sasum_(n, &z__[1], &c__1);
    sscal_(n, &s, &z__[1], &c__1);
/*                                 ** solve L*V = Y */
    ynorm = 1.f;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	l = ipvt[k];
	t = z__[l];
	z__[l] = z__[k];
	z__[k] = t;
	if (k < *n) {
	    i__2 = *n - k;
	    saxpy_(&i__2, &t, &a[k + 1 + k * a_dim1], &c__1, &z__[k + 1], &
		    c__1);
	}
	if ((r__1 = z__[k], dabs(r__1)) > 1.f) {
	    s = 1.f / (r__1 = z__[k], dabs(r__1));
	    sscal_(n, &s, &z__[1], &c__1);
	    ynorm = s * ynorm;
	}
/* L70: */
    }
    s = 1.f / sasum_(n, &z__[1], &c__1);
    sscal_(n, &s, &z__[1], &c__1);
/*                                  ** solve  U*Z = V */
    ynorm = s * ynorm;
    i__1 = *n;
    for (kb = 1; kb <= i__1; ++kb) {
	k = *n + 1 - kb;
	if ((r__1 = z__[k], dabs(r__1)) > (r__2 = a[k + k * a_dim1], dabs(
		r__2))) {
	    s = (r__1 = a[k + k * a_dim1], dabs(r__1)) / (r__2 = z__[k], dabs(
		    r__2));
	    sscal_(n, &s, &z__[1], &c__1);
	    ynorm = s * ynorm;
	}
	if (a[k + k * a_dim1] != 0.f) {
	    z__[k] /= a[k + k * a_dim1];
	}
	if (a[k + k * a_dim1] == 0.f) {
	    z__[k] = 1.f;
	}
	t = -z__[k];
	i__2 = k - 1;
	saxpy_(&i__2, &t, &a[k * a_dim1 + 1], &c__1, &z__[1], &c__1);
/* L80: */
    }
/*                                   ** make znorm = 1.0 */
    s = 1.f / sasum_(n, &z__[1], &c__1);
    sscal_(n, &s, &z__[1], &c__1);
    ynorm = s * ynorm;
    if (anorm != 0.f) {
	*rcond = ynorm / anorm;
    }
    if (anorm == 0.f) {
	*rcond = 0.f;
    }
    return 0;
} /* sgeco_ */

/* Subroutine */ int sgefa_(real *a, integer *lda, integer *n, integer *ipvt, 
	integer *info)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer j, k, l;
    static real t;
    static integer kp1, nm1;
    extern /* Subroutine */ int sscal_(integer *, real *, real *, integer *), 
	    saxpy_(integer *, real *, real *, integer *, real *, integer *);
    extern integer isamax_(integer *, real *, integer *);

/*         Factors a real matrix by Gaussian elimination. */
/*         Revision date:  8/1/82 */
/*         Author:  Moler, C. B. (U. of New Mexico) */
/*     SGEFA is usually called by SGECO, but it can be called */
/*     directly with a saving in time if  RCOND  is not needed. */
/*     (time for SGECO) = (1 + 9/N) * (time for SGEFA) . */
/*     Input:  same as SGECO */
/*     On return: */
/*        A,IPVT  same as SGECO */
/*        INFO    INTEGER */
/*                = 0  normal value. */
/*                = k  if  u(k,k) .eq. 0.0 .  This is not an error */
/*                     condition for this subroutine, but it does */
/*                     indicate that SGESL or SGEDI will divide by zero */
/*                     if called.  Use  RCOND  in SGECO for a reliable */
/*                     indication of singularity. */
/* --------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
/*                      ** Gaussian elimination with partial pivoting */
    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ipvt;

    /* Function Body */
    *info = 0;
    nm1 = *n - 1;
    i__1 = nm1;
    for (k = 1; k <= i__1; ++k) {
	kp1 = k + 1;
/*                                            ** find L = pivot index */
	i__2 = *n - k + 1;
	l = isamax_(&i__2, &a[k + k * a_dim1], &c__1) + k - 1;
	ipvt[k] = l;
	if (a[l + k * a_dim1] == 0.f) {
/*                                     ** zero pivot implies this column */
/*                                     ** already triangularized */
	    *info = k;
	} else {
/*                                     ** interchange if necessary */
	    if (l != k) {
		t = a[l + k * a_dim1];
		a[l + k * a_dim1] = a[k + k * a_dim1];
		a[k + k * a_dim1] = t;
	    }
/*                                     ** compute multipliers */
	    t = -1.f / a[k + k * a_dim1];
	    i__2 = *n - k;
	    sscal_(&i__2, &t, &a[k + 1 + k * a_dim1], &c__1);
/*                              ** row elimination with column indexing */
	    i__2 = *n;
	    for (j = kp1; j <= i__2; ++j) {
		t = a[l + j * a_dim1];
		if (l != k) {
		    a[l + j * a_dim1] = a[k + j * a_dim1];
		    a[k + j * a_dim1] = t;
		}
		i__3 = *n - k;
		saxpy_(&i__3, &t, &a[k + 1 + k * a_dim1], &c__1, &a[k + 1 + j 
			* a_dim1], &c__1);
/* L10: */
	    }
	}
/* L20: */
    }
    ipvt[*n] = *n;
    if (a[*n + *n * a_dim1] == 0.f) {
	*info = *n;
    }
    return 0;
} /* sgefa_ */

/* Subroutine */ int sgesl_(real *a, integer *lda, integer *n, integer *ipvt, 
	real *b, integer *job)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2;

    /* Local variables */
    static integer k, l;
    static real t;
    static integer kb, nm1;
    extern doublereal sdot_(integer *, real *, integer *, real *, integer *);
    extern /* Subroutine */ int saxpy_(integer *, real *, real *, integer *, 
	    real *, integer *);

/*         Solves the real system */
/*            A * X = B  or  transpose(A) * X = B */
/*         using the factors computed by SGECO or SGEFA. */
/*         Revision date:  8/1/82 */
/*         Author:  Moler, C. B. (U. of New Mexico) */
/*     On entry */
/*        A       REAL(LDA, N) */
/*                the output from SGECO or SGEFA. */
/*        LDA     INTEGER */
/*                the leading dimension of the array  A */
/*        N       INTEGER */
/*                the order of the matrix  A */
/*        IPVT    INTEGER(N) */
/*                the pivot vector from SGECO or SGEFA. */
/*        B       REAL(N) */
/*                the right hand side vector. */
/*        JOB     INTEGER */
/*                = 0         to solve  A*X = B , */
/*                = nonzero   to solve  transpose(A)*X = B */
/*     On return */
/*        B       the solution vector  X */
/*     Error condition */
/*        A division by zero will occur if the input factor contains a */
/*        zero on the diagonal.  Technically, this indicates singularity, */
/*        but it is often caused by improper arguments or improper */
/*        setting of LDA.  It will not occur if the subroutines are */
/*        called correctly and if SGECO has set RCOND .GT. 0.0 */
/*        or SGEFA has set INFO .EQ. 0 . */
/*     To compute  inverse(a) * c  where  c  is a matrix */
/*     with  p  columns */
/*           call sgeco(a,lda,n,ipvt,rcond,z) */
/*           if (rcond is too small) go to ... */
/*           do 10 j = 1, p */
/*              call sgesl(a,lda,n,ipvt,c(1,j),0) */
/*        10 continue */
/* --------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. External Subroutines .. */
/*     .. */
    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ipvt;
    --b;

    /* Function Body */
    nm1 = *n - 1;
    if (*job == 0) {
/*                                 ** solve  A * X = B */
/*                                     ** first solve  L*Y = B */
	i__1 = nm1;
	for (k = 1; k <= i__1; ++k) {
	    l = ipvt[k];
	    t = b[l];
	    if (l != k) {
		b[l] = b[k];
		b[k] = t;
	    }
	    i__2 = *n - k;
	    saxpy_(&i__2, &t, &a[k + 1 + k * a_dim1], &c__1, &b[k + 1], &c__1)
		    ;
/* L10: */
	}
/*                                    ** now solve  U*X = Y */
	i__1 = *n;
	for (kb = 1; kb <= i__1; ++kb) {
	    k = *n + 1 - kb;
	    b[k] /= a[k + k * a_dim1];
	    t = -b[k];
	    i__2 = k - 1;
	    saxpy_(&i__2, &t, &a[k * a_dim1 + 1], &c__1, &b[1], &c__1);
/* L20: */
	}
    } else {
/*                         ** solve  trans(A) * X = B */
/*                                    ** first solve  trans(U)*Y = B */
	i__1 = *n;
	for (k = 1; k <= i__1; ++k) {
	    i__2 = k - 1;
	    t = sdot_(&i__2, &a[k * a_dim1 + 1], &c__1, &b[1], &c__1);
	    b[k] = (b[k] - t) / a[k + k * a_dim1];
/* L30: */
	}
/*                                    ** now solve  trans(l)*x = y */
	i__1 = nm1;
	for (kb = 1; kb <= i__1; ++kb) {
	    k = *n - kb;
	    i__2 = *n - k;
	    b[k] += sdot_(&i__2, &a[k + 1 + k * a_dim1], &c__1, &b[k + 1], &
		    c__1);
	    l = ipvt[k];
	    if (l != k) {
		t = b[l];
		b[l] = b[k];
		b[k] = t;
	    }
/* L40: */
	}
    }
    return 0;
} /* sgesl_ */

doublereal sasum_(integer *n, real *sx, integer *incx)
{
    /* System generated locals */
    integer i__1, i__2;
    real ret_val, r__1, r__2, r__3, r__4, r__5, r__6;

    /* Local variables */
    static integer i__, m;

/*  INPUT--    N  Number of elements in vector to be summed */
/*            SX  Sing-prec array, length 1+(N-1)*INCX, containing vector */
/*          INCX  Spacing of vector elements in SX */
/*  OUTPUT-- SASUM   Sum from 0 to N-1 of  ABS(SX(1+I*INCX)) */
/* ---------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    /* Parameter adjustments */
    --sx;

    /* Function Body */
    ret_val = 0.f;
    if (*n <= 0) {
	return ret_val;
    }
    if (*incx != 1) {
/*                                          ** non-unit increments */
	i__1 = (*n - 1) * *incx + 1;
	i__2 = *incx;
	for (i__ = 1; i__2 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__2) {
	    ret_val += (r__1 = sx[i__], dabs(r__1));
/* L10: */
	}
    } else {
/*                                          ** unit increments */
	m = *n % 6;
	if (m != 0) {
/*                             ** clean-up loop so remaining vector */
/*                             ** length is a multiple of 6. */
	    i__2 = m;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		ret_val += (r__1 = sx[i__], dabs(r__1));
/* L20: */
	    }
	}
/*                              ** unroll loop for speed */
	i__2 = *n;
	for (i__ = m + 1; i__ <= i__2; i__ += 6) {
	    ret_val = ret_val + (r__1 = sx[i__], dabs(r__1)) + (r__2 = sx[i__ 
		    + 1], dabs(r__2)) + (r__3 = sx[i__ + 2], dabs(r__3)) + (
		    r__4 = sx[i__ + 3], dabs(r__4)) + (r__5 = sx[i__ + 4], 
		    dabs(r__5)) + (r__6 = sx[i__ + 5], dabs(r__6));
/* L30: */
	}
    }
    return ret_val;
} /* sasum_ */

/* Subroutine */ int saxpy_(integer *n, real *sa, real *sx, integer *incx, 
	real *sy, integer *incy)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, m, ix, iy;

/*          Y = A*X + Y  (X, Y = vectors, A = scalar) */
/*  INPUT-- */
/*        N  Number of elements in input vectors X and Y */
/*       SA  Single precision scalar multiplier A */
/*       SX  Sing-prec array containing vector X */
/*     INCX  Spacing of elements of vector X in SX */
/*       SY  Sing-prec array containing vector Y */
/*     INCY  Spacing of elements of vector Y in SY */
/* OUTPUT-- */
/*       SY   For I = 0 to N-1, overwrite  SY(LY+I*INCY) with */
/*                 SA*SX(LX+I*INCX) + SY(LY+I*INCY), */
/*            where LX = 1          if INCX .GE. 0, */
/*                     = (-INCX)*N  if INCX .LT. 0 */
/*            and LY is defined analogously using INCY. */
/* ------------------------------------------------------------ */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    /* Parameter adjustments */
    --sy;
    --sx;

    /* Function Body */
    if (*n <= 0 || *sa == 0.f) {
	return 0;
    }
    if (*incx == *incy && *incx > 1) {
	i__1 = (*n - 1) * *incx + 1;
	i__2 = *incx;
	for (i__ = 1; i__2 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__2) {
	    sy[i__] += *sa * sx[i__];
/* L10: */
	}
    } else if (*incx == *incy && *incx == 1) {
/*                                        ** equal, unit increments */
	m = *n % 4;
	if (m != 0) {
/*                            ** clean-up loop so remaining vector length */
/*                            ** is a multiple of 4. */
	    i__2 = m;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		sy[i__] += *sa * sx[i__];
/* L20: */
	    }
	}
/*                              ** unroll loop for speed */
	i__2 = *n;
	for (i__ = m + 1; i__ <= i__2; i__ += 4) {
	    sy[i__] += *sa * sx[i__];
	    sy[i__ + 1] += *sa * sx[i__ + 1];
	    sy[i__ + 2] += *sa * sx[i__ + 2];
	    sy[i__ + 3] += *sa * sx[i__ + 3];
/* L30: */
	}
    } else {
/*               ** nonequal or nonpositive increments. */
	ix = 1;
	iy = 1;
	if (*incx < 0) {
	    ix = (*n - 1) * (-(*incx)) + 1;
	}
	if (*incy < 0) {
	    iy = (*n - 1) * (-(*incy)) + 1;
	}
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    sy[iy] += *sa * sx[ix];
	    ix += *incx;
	    iy += *incy;
/* L40: */
	}
    }
    return 0;
} /* saxpy_ */

doublereal sdot_(integer *n, real *sx, integer *incx, real *sy, integer *incy)
{
    /* System generated locals */
    integer i__1, i__2;
    real ret_val;

    /* Local variables */
    static integer i__, m, ix, iy;

/*        Single-prec dot product of vectors  X  and  Y */
/*  INPUT-- */
/*        N  Number of elements in input vectors X and Y */
/*       SX  Sing-prec array containing vector X */
/*     INCX  Spacing of elements of vector X in SX */
/*       SY  Sing-prec array containing vector Y */
/*     INCY  Spacing of elements of vector Y in SY */
/* OUTPUT-- */
/*     SDOT   Sum for I = 0 to N-1 of  SX(LX+I*INCX) * SY(LY+I*INCY), */
/*            where  LX = 1          if INCX .GE. 0, */
/*                      = (-INCX)*N  if INCX .LT. 0, */
/*            and LY is defined analogously using INCY. */
/* ------------------------------------------------------------------ */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    /* Parameter adjustments */
    --sy;
    --sx;

    /* Function Body */
    ret_val = 0.f;
    if (*n <= 0) {
	return ret_val;
    }
    if (*incx == *incy && *incx > 1) {
	i__1 = (*n - 1) * *incx + 1;
	i__2 = *incx;
	for (i__ = 1; i__2 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__2) {
	    ret_val += sx[i__] * sy[i__];
/* L10: */
	}
    } else if (*incx == *incy && *incx == 1) {
/*                                        ** equal, unit increments */
	m = *n % 5;
	if (m != 0) {
/*                            ** clean-up loop so remaining vector length */
/*                            ** is a multiple of 4. */
	    i__2 = m;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		ret_val += sx[i__] * sy[i__];
/* L20: */
	    }
	}
/*                              ** unroll loop for speed */
	i__2 = *n;
	for (i__ = m + 1; i__ <= i__2; i__ += 5) {
	    ret_val = ret_val + sx[i__] * sy[i__] + sx[i__ + 1] * sy[i__ + 1] 
		    + sx[i__ + 2] * sy[i__ + 2] + sx[i__ + 3] * sy[i__ + 3] + 
		    sx[i__ + 4] * sy[i__ + 4];
/* L30: */
	}
    } else {
/*               ** nonequal or nonpositive increments. */
	ix = 1;
	iy = 1;
	if (*incx < 0) {
	    ix = (*n - 1) * (-(*incx)) + 1;
	}
	if (*incy < 0) {
	    iy = (*n - 1) * (-(*incy)) + 1;
	}
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    ret_val += sx[ix] * sy[iy];
	    ix += *incx;
	    iy += *incy;
/* L40: */
	}
    }
    return ret_val;
} /* sdot_ */

/* Subroutine */ int sscal_(integer *n, real *sa, real *sx, integer *incx)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, m;

/*         Multiply vector SX by scalar SA */
/*  INPUT--  N  Number of elements in vector */
/*          SA  Single precision scale factor */
/*          SX  Sing-prec array, length 1+(N-1)*INCX, containing vector */
/*        INCX  Spacing of vector elements in SX */
/* OUTPUT-- SX  Replace  SX(1+I*INCX)  with  SA * SX(1+I*INCX) */
/*                for I = 0 to N-1 */
/* --------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    /* Parameter adjustments */
    --sx;

    /* Function Body */
    if (*n <= 0) {
	return 0;
    }
    if (*incx != 1) {
	i__1 = (*n - 1) * *incx + 1;
	i__2 = *incx;
	for (i__ = 1; i__2 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__2) {
	    sx[i__] = *sa * sx[i__];
/* L10: */
	}
    } else {
	m = *n % 5;
	if (m != 0) {
/*                           ** clean-up loop so remaining vector length */
/*                           ** is a multiple of 5. */
	    i__2 = m;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		sx[i__] = *sa * sx[i__];
/* L20: */
	    }
	}
/*                             ** unroll loop for speed */
	i__2 = *n;
	for (i__ = m + 1; i__ <= i__2; i__ += 5) {
	    sx[i__] = *sa * sx[i__];
	    sx[i__ + 1] = *sa * sx[i__ + 1];
	    sx[i__ + 2] = *sa * sx[i__ + 2];
	    sx[i__ + 3] = *sa * sx[i__ + 3];
	    sx[i__ + 4] = *sa * sx[i__ + 4];
/* L30: */
	}
    }
    return 0;
} /* sscal_ */

/* Subroutine */ int sswap_(integer *n, real *sx, integer *incx, real *sy, 
	integer *incy)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, m, ix, iy;
    static real stemp1, stemp2, stemp3;

/*          Interchange s.p vectors  X  and  Y, as follows: */
/*     For I = 0 to N-1, interchange  SX(LX+I*INCX) and SY(LY+I*INCY), */
/*     where LX = 1          if INCX .GE. 0, */
/*              = (-INCX)*N  if INCX .LT. 0 */
/*     and LY is defined analogously using INCY. */
/*  INPUT-- */
/*        N  Number of elements in input vectors X and Y */
/*       SX  Sing-prec array containing vector X */
/*     INCX  Spacing of elements of vector X in SX */
/*       SY  Sing-prec array containing vector Y */
/*     INCY  Spacing of elements of vector Y in SY */
/* OUTPUT-- */
/*       SX  Input vector SY (unchanged if N .LE. 0) */
/*       SY  Input vector SX (unchanged IF N .LE. 0) */
/* -------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    /* Parameter adjustments */
    --sy;
    --sx;

    /* Function Body */
    if (*n <= 0) {
	return 0;
    }
    if (*incx == *incy && *incx > 1) {
	i__1 = (*n - 1) * *incx + 1;
	i__2 = *incx;
	for (i__ = 1; i__2 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__2) {
	    stemp1 = sx[i__];
	    sx[i__] = sy[i__];
	    sy[i__] = stemp1;
/* L10: */
	}
    } else if (*incx == *incy && *incx == 1) {
/*                                        ** equal, unit increments */
	m = *n % 3;
	if (m != 0) {
/*                            ** clean-up loop so remaining vector length */
/*                            ** is a multiple of 3. */
	    i__2 = m;
	    for (i__ = 1; i__ <= i__2; ++i__) {
		stemp1 = sx[i__];
		sx[i__] = sy[i__];
		sy[i__] = stemp1;
/* L20: */
	    }
	}
/*                              ** unroll loop for speed */
	i__2 = *n;
	for (i__ = m + 1; i__ <= i__2; i__ += 3) {
	    stemp1 = sx[i__];
	    stemp2 = sx[i__ + 1];
	    stemp3 = sx[i__ + 2];
	    sx[i__] = sy[i__];
	    sx[i__ + 1] = sy[i__ + 1];
	    sx[i__ + 2] = sy[i__ + 2];
	    sy[i__] = stemp1;
	    sy[i__ + 1] = stemp2;
	    sy[i__ + 2] = stemp3;
/* L30: */
	}
    } else {
/*               ** nonequal or nonpositive increments. */
	ix = 1;
	iy = 1;
	if (*incx < 0) {
	    ix = (*n - 1) * (-(*incx)) + 1;
	}
	if (*incy < 0) {
	    iy = (*n - 1) * (-(*incy)) + 1;
	}
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
	    stemp1 = sx[ix];
	    sx[ix] = sy[iy];
	    sy[iy] = stemp1;
	    ix += *incx;
	    iy += *incy;
/* L40: */
	}
    }
    return 0;
} /* sswap_ */

integer isamax_(integer *n, real *sx, integer *incx)
{
    /* System generated locals */
    integer ret_val, i__1, i__2;
    real r__1;

    /* Local variables */
    static integer i__, ii;
    static real xmag, smax;

/* INPUT--  N     Number of elements in vector of interest */
/*          SX    Sing-prec array, length 1+(N-1)*INCX, containing vector */
/*          INCX  Spacing of vector elements in SX */
/* OUTPUT-- ISAMAX   First I, I = 1 to N, to maximize */
/*                         ABS(SX(1+(I-1)*INCX)) */
/* --------------------------------------------------------------------- */
/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */
/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
    /* Parameter adjustments */
    --sx;

    /* Function Body */
    if (*n <= 0) {
	ret_val = 0;
    } else if (*n == 1) {
	ret_val = 1;
    } else {
	smax = 0.f;
	ii = 1;
	i__1 = (*n - 1) * *incx + 1;
	i__2 = *incx;
	for (i__ = 1; i__2 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__2) {
	    xmag = (r__1 = sx[i__], dabs(r__1));
	    if (smax < xmag) {
		smax = xmag;
		ret_val = ii;
	    }
	    ++ii;
/* L10: */
	}
    }
    return ret_val;
} /* isamax_ */

