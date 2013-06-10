/* 
 * Functions.h
 *
 * Some useful kinematics functions for coherent two-body
 * photoproduction of a neutral pion (or any other particle).
 *
 * 22.10.2006
 *
 * DLH 
 *
 */

// Calculates minimum gamma-gamma opening angle.
Double_t Tgg_Min( Double_t T, Double_t mpi)
{
	Double_t term, tgg_min;

	term = mpi/sqrt(T*(T+2*mpi));
	tgg_min = 2*atan(term);

	return( tgg_min);
}

// Calculates maximum KE of the pion.
Double_t qT_max( Double_t eg, Double_t pm, Double_t qm)
{
	Double_t W, W_cm, S, beta, gamma;
	Double_t qcm_e, qcm_p, qcm_x, qcm_z;
	Double_t qx, qz, qp, qe, qT_max, th_cm;

	W = eg + pm;
	S = Sqr( pm) + 2*eg*pm;
	W_cm = sqrt( S);

	beta = eg/W;
	gamma = W/W_cm;

	qcm_e = (S + Sqr(qm) - Sqr(pm))/2/W_cm;
	qcm_p = Momentum( qcm_e, qm);

	th_cm = 0;  // Condition for max pion KE

	qcm_x = qcm_p*sin( th_cm);
	qcm_z = qcm_p*cos( th_cm);
	qx = qcm_x;
	qz = gamma*(qcm_z + beta*qcm_e);
	qp = sqrt( Sqr( qx) + Sqr(qz));
	qe = Energy( qp, qm);
	qT_max = qe - qm;

	return( qT_max);
}

// Calculates relativistic energy from mass and momentum.
Double_t Energy( Double_t mom, Double_t m)
{
	return( sqrt(mom*mom + m*m));
}

// Calculates relativistic momentum from mass and energy.
Double_t Momentum( Double_t en, Double_t m)
{
	if ( en >= m) return( sqrt( en*en - m*m));
	else return( -1);
}

// Calculates the square.
Double_t Sqr( Double_t x)
{
	return( x*x);
}

// Calculates the threshold energy.
Double_t E_thr( Double_t pm, Double_t qm)
{
	return( (Sqr( qm) + 2*qm*pm)/2/pm);
}

// Does a simple linear interpolation.
Double_t Linear( Double_t x0, Double_t y0, Double_t x1, Double_t y1, Double_t x)
{
	return( y0 + (y1 - y0)/(x1 - x0)*(x - x0));
}

// Calculates the lab momentum of the pion.
// This one takes the pion LAB theta!
Double_t qp( Double_t ke, Double_t pm, Double_t qth, Double_t qm)
{
	Double_t qmom;
	Double_t W;
	Double_t A, B, C, a, b, c;

	W = ke + pm;

	A = 2*ke*pm + Sqr( qm);
	B = 2*ke*cos( qth);
	C = 2*W;

	a = Sqr( C) - Sqr( B);
	b = -2*A*B;
	c = Sqr( C*qm) - Sqr( A);

	qmom = (-b + sqrt( Sqr( b) - 4*a*c))/2/a;

	if ( qmom != qmom) return( -1);
	else return( qmom);
}

// Calculates the lab momentum of the pion.
//
// Takes the pion CM theta in degrees, but still calculates the lab momentum!
//
Double_t qp_thcm( Double_t ke, Double_t pm, Double_t qth_cm, Double_t qm)
{

	TLorentzVector k, p, q, pIn;
	TLorentzVector k_cm, p_cm, q_cm;
	TVector3 cmBoost, labBoost, q3mom;
	
	Double_t qm, qph_cm, mom_cm;
	Double_t qx_cm, qy_cm, qz_cm, qe_cm;
	Double_t pm, S;

	k.SetPxPyPzE( 0, 0, ke, ke);
	p.SetPxPyPzE( 0, 0, 0, pm);

	pIn = k + p;
	labBoost = pIn.BoostVector();
	cmBoost = -pIn.BoostVector();

	k_cm = k;
	k_cm.Boost( cmBoost);
	p_cm = p;
	p_cm.Boost( cmBoost);

	qth_cm *= kD2R;
	qph_cm = 0;

	S = 2*ke*pm + pm*pm;
	qe_cm = (S - pm*pm + qm*qm)/2/sqrt(S);

	mom_cm = Momentum( qe_cm, qm);
	qx_cm = mom_cm*sin( qth_cm)*cos(qph_cm);
	qy_cm = mom_cm*sin( qth_cm)*sin(qph_cm);
	qz_cm = mom_cm*cos( qth_cm);

	q_cm.SetPxPyPzE( qx_cm, qy_cm, qz_cm, qe_cm);

	q = q_cm;
	q.Boost( labBoost);

	q3mom = q.Vect();

	return( q3mom.Mag());
}

// Calculates the recoil momentum of the target.  It uses the above
// function qp.
Double_t ppr( Double_t ke, Double_t pm, Double_t qth, Double_t qm)
{
	Double_t ppx, ppz, qx, qz, q, pp;

	q = qp( ke, pm, qth, qm);
	qz = q*cos( qth);
	qx = q*sin( qth);

	ppz = ke - qz;
	ppx = -qx;

	pp = sqrt( Sqr( ppx) + Sqr( ppz));

	return( pp);
}

// Divides canvas into a bunch of little pads

void DividegPad( UInt_t nx, UInt_t ny, Double_t l, Double_t r, Double_t t, Double_t b)
{
   UInt_t ix, iy, n = 0;
   Double_t x1, x2, y1, y2;
   Double_t dx = ((1-r)*(1-l))/((1-r)*(1-l)*(nx-2)-r+2-l);
   Double_t dl = dx/(1-l);
   Double_t dy = ((1-t)*(1-b))/((1-t)*(1-b)*(ny-2)-t+2-b);
   Double_t db = dy/(1-b);
   char *name  = new char [strlen(gPad->GetName())+6];

   y1 = 0;
   y2 = db;
   for ( iy = 0; iy < ny; iy++) {
      x1 = 0;
      x2 = dl;
      for ( ix = 0; ix < nx; ix++) {
         if (x1 > x2) continue;
         n++;
         sprintf( name, "%s_%d", gPad->GetName(), n);
         pad = new TPad( name, name, x1, y1, x2, y2, 0);
         if (ix==0) pad->SetLeftMargin(l);
         if (ix==nx-1) pad->SetRightMargin(r);
         if (iy==ny-1) pad->SetTopMargin(t);
         if (iy==0) pad->SetBottomMargin(b);
         x1 = x2;
         if (ix==nx-2) x2 = 1;
         else x2 = x1+dx;
         pad->SetNumber( n);
         pad->Draw();
      }
      y1 = y2;
      if ( iy == ny-2) y2 = 1;
      else y2 = y1+dy;
   }
}
