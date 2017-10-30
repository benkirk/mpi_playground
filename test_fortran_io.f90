! -----------------------------------------------------------------------
      subroutine write_f (rank, length, rdata)

      integer rank, length
      integer, parameter:: dp=kind(0.d0)
      real(dp), intent(in) :: rdata(length)

      !write(*,*) "Write from Fortran!", rank, length, rdata(1)

      end subroutine



! -----------------------------------------------------------------------
      subroutine read_f (rank, length, rdata)

      integer rank, length
      integer, parameter:: dp=kind(0.d0)
      real(dp), intent(out) :: rdata(length)

      !write(*,*) "Read  from Fortran!", rank, length

      end subroutine
