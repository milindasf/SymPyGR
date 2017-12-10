  adv_deriv_x(adx_gtxx,gtxx,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_gtxy,gtxy,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_gtxz,gtxz,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_gtyy,gtyy,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_gtyz,gtyz,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_gtzz,gtzz,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Atxx,Atxx,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Atxy,Atxy,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Atxz,Atxz,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Atyy,Atyy,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Atyz,Atyz,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Atzz,Atzz,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_alpha,alpha,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_shiftx,shiftx,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_shifty,shifty,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_shiftz,shiftz,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_chi,chi,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Gamtx,Gamtx,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Gamty,Gamty,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_Gamtz,Gamtz,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_trK,trK,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_gbx,gbx,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_gby,gby,dx, nx, ny, nz, shiftx);
  adv_deriv_x(adx_gbz,gbz,dx, nx, ny, nz, shiftx);
  double *beta_t = new double [nd];
  transpose_xy(beta_t, shifty, nx, ny, nz);
  transpose_xy(ft, gtxx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gtxx,dft, ny, nx, nz);
  transpose_xy(ft, gtxy,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gtxy,dft, ny, nx, nz);
  transpose_xy(ft, gtxz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gtxz,dft, ny, nx, nz);
  transpose_xy(ft, gtyy,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gtyy,dft, ny, nx, nz);
  transpose_xy(ft, gtyz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gtyz,dft, ny, nx, nz);
  transpose_xy(ft, gtzz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gtzz,dft, ny, nx, nz);
  transpose_xy(ft, Atxx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Atxx,dft, ny, nx, nz);
  transpose_xy(ft, Atxy,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Atxy,dft, ny, nx, nz);
  transpose_xy(ft, Atxz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Atxz,dft, ny, nx, nz);
  transpose_xy(ft, Atyy,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Atyy,dft, ny, nx, nz);
  transpose_xy(ft, Atyz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Atyz,dft, ny, nx, nz);
  transpose_xy(ft, Atzz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Atzz,dft, ny, nx, nz);
  transpose_xy(ft, alpha,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_alpha,dft, ny, nx, nz);
  transpose_xy(ft, shiftx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_shiftx,dft, ny, nx, nz);
  transpose_xy(ft, shifty,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_shifty,dft, ny, nx, nz);
  transpose_xy(ft, shiftz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_shiftz,dft, ny, nx, nz);
  transpose_xy(ft, chi,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_chi,dft, ny, nx, nz);
  transpose_xy(ft, Gamtx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Gamtx,dft, ny, nx, nz);
  transpose_xy(ft, Gamty,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Gamty,dft, ny, nx, nz);
  transpose_xy(ft, Gamtz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_Gamtz,dft, ny, nx, nz);
  transpose_xy(ft, trK,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_trK,dft, ny, nx, nz);
  transpose_xy(ft, gbx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gbx,dft, ny, nx, nz);
  transpose_xy(ft, gby,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gby,dft, ny, nx, nz);
  transpose_xy(ft, gbz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, ny, nx, nz, beta_t);
  transpose_xy(ady_gbz,dft, ny, nx, nz);
  transpose_xz(beta_t, shiftz, nx, ny, nz);
  transpose_xz(ft, gtxx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gtxx,dft,nz, ny, nx);
  transpose_xz(ft, gtxy,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gtxy,dft,nz, ny, nx);
  transpose_xz(ft, gtxz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gtxz,dft,nz, ny, nx);
  transpose_xz(ft, gtyy,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gtyy,dft,nz, ny, nx);
  transpose_xz(ft, gtyz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gtyz,dft,nz, ny, nx);
  transpose_xz(ft, gtzz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gtzz,dft,nz, ny, nx);
  transpose_xz(ft, Atxx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Atxx,dft,nz, ny, nx);
  transpose_xz(ft, Atxy,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Atxy,dft,nz, ny, nx);
  transpose_xz(ft, Atxz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Atxz,dft,nz, ny, nx);
  transpose_xz(ft, Atyy,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Atyy,dft,nz, ny, nx);
  transpose_xz(ft, Atyz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Atyz,dft,nz, ny, nx);
  transpose_xz(ft, Atzz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Atzz,dft,nz, ny, nx);
  transpose_xz(ft, alpha,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_alpha,dft,nz, ny, nx);
  transpose_xz(ft, shiftx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_shiftx,dft,nz, ny, nx);
  transpose_xz(ft, shifty,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_shifty,dft,nz, ny, nx);
  transpose_xz(ft, shiftz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_shiftz,dft,nz, ny, nx);
  transpose_xz(ft, chi,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_chi,dft,nz, ny, nx);
  transpose_xz(ft, Gamtx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Gamtx,dft,nz, ny, nx);
  transpose_xz(ft, Gamty,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Gamty,dft,nz, ny, nx);
  transpose_xz(ft, Gamtz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_Gamtz,dft,nz, ny, nx);
  transpose_xz(ft, trK,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_trK,dft,nz, ny, nx);
  transpose_xz(ft, gbx,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gbx,dft,nz, ny, nx);
  transpose_xz(ft, gby,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gby,dft,nz, ny, nx);
  transpose_xz(ft, gbz,nx, ny, nz);
  adv_deriv_x(dft, ft, dy, nz, ny, nx, beta_t);
  transpose_xz(adz_gbz,dft,nz, ny, nx);
  delete [] beta_t;
  delete [] ft;
  delete [] dft;
