! -----------------------------------------------------------------------
subroutine my_filename (rank, fname)

   implicit none
   integer rank
   character(len=24) fname

   write(fname,10) rank
10 format('test.'i0.3,'.fbin')
   !write(*,*) fname

end subroutine my_filename



! -----------------------------------------------------------------------
subroutine write_f (rank, length, rdata)

  implicit none
  integer rank, length, u, i
  integer, parameter:: dp=kind(0.d0)
  character(len=24) fname
  real(dp), intent(in) :: rdata(length)

 !write(*,*) "Write from Fortran!", rank, length, rdata(1)

  call my_filename(rank,fname)
  open(newunit=u,file=fname,form='unformatted',status='replace')
  write(u) rdata !(rdata(i), i=1,length)
  flush(u)
  close(u)

end subroutine write_f



! -----------------------------------------------------------------------
subroutine read_f (rank, length, rdata)

  implicit none
  integer rank, length, u
  integer, parameter:: dp=kind(0.d0)
  character(len=24) fname
  real(dp), intent(out) :: rdata(length)

  !write(*,*) "Read  from Fortran!", rank, length

  call my_filename(rank,fname)
  open(newunit=u,file=fname,form='unformatted',status='old')
  read(u) rdata !(rdata(i), i=1,length)
  close(u)

end subroutine read_f
