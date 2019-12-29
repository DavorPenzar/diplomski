#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/boolean.h"
#include "include/numeric.h"

#define LEN_MAX 1024U

size_t compute_ld (size_t m, size_t n)
{
    return m ? (((m + 63U) >> 6U) << 6U) : 1U;
}

size_t index (size_t ld, size_t i, size_t j)
{
    return j * ld + i;
}

extern void dgesvd_ (
    char* JOBU,
    char* JOBVT,
    int* M,
    int* N,
    double* A,
    int* LDA,
    double* S,
    double* U,
    int* LDU,
    double* VT,
    int* LDVT,
    double* WORK,
    int* LWORK,
    int* INFO
);

extern void dgesdd_ (
    char* JOBZ,
    int* M,
    int* N,
    double* A,
    int* LDA,
    double* S,
    double* U,
    int* LDU,
    double* VT,
    int* LDVT,
    double* WORK,
    int* LWORK,
    int* IWORK,
    int* INFO
);

void dgesvd_wrapper (
    size_t m,
    size_t n,
    real_t* A,
    size_t ld_A,
    real_t* U,
    size_t ld_U,
    real_t* Vt,
    size_t ld_Vt,
    real_t* s,
    job_t job_U,
    job_t job_Vt,
    int lwork,
    double* work,
    int* info
)
{
    int int_m;
    int int_n;

    int int_ld_A;
    int int_ld_U;
    int int_ld_Vt;

    char char_job_U;
    char char_job_Vt;

    int_m = 0;
    int_n = 0;

    int_ld_A = 0;
    int_ld_U = 0;
    int_ld_Vt = 0;

    char_job_U = '\0';
    char_job_Vt = '\0';

    int_m = (int)m;
    int_n = (int)n;

    int_ld_A = (int)ld_A;
    int_ld_U = (int)ld_U;
    int_ld_Vt = (int)ld_Vt;

    switch (job_U)
    {
        case all:
            char_job_U = 'A';

            break;

        case first:
            char_job_U = 'S';

            break;

        case over:
            char_job_U = 'O';

            break;

        case none:
            char_job_U = 'N';

            break;

        default:
            break;
    }
    switch (job_Vt)
    {
        case all:
            char_job_Vt = 'A';

            break;

        case first:
            char_job_Vt = 'S';

            break;

        case over:
            char_job_Vt = 'O';

            break;

        case none:
            char_job_Vt = 'N';

            break;

        default:
            break;
    }

    dgesvd_(
        &char_job_U,
        &char_job_Vt,
        &int_m,
        &int_n,
        A,
        &int_ld_A,
        s,
        U,
        &int_ld_U,
        Vt,
        &int_ld_Vt,
        work,
        &lwork,
        info
    );
}

void dgesdd_wrapper (
    size_t m,
    size_t n,
    real_t* A,
    size_t ld_A,
    real_t* U,
    size_t ld_U,
    real_t* Vt,
    size_t ld_Vt,
    real_t* s,
    job_t job_Z,
    int lwork,
    double* work,
    int* iwork,
    int* info
)
{
    int int_m;
    int int_n;

    int int_ld_A;
    int int_ld_U;
    int int_ld_Vt;

    char char_job_Z;

    int_m = 0;
    int_n = 0;

    int_ld_A = 0;
    int_ld_U = 0;
    int_ld_Vt = 0;

    char_job_Z = '\0';

    int_m = (int)m;
    int_n = (int)n;

    int_ld_A = (int)ld_A;
    int_ld_U = (int)ld_U;
    int_ld_Vt = (int)ld_Vt;

    switch (job_Z)
    {
        case all:
            char_job_Z = 'A';

            break;

        case first:
            char_job_Z = 'S';

            break;

        case over:
            char_job_Z = 'O';

            break;

        case none:
            char_job_Z = 'N';

            break;

        default:
            break;
    }

    dgesdd_(
        &char_job_Z,
        &int_m,
        &int_n,
        A,
        &int_ld_A,
        s,
        U,
        &int_ld_U,
        Vt,
        &int_ld_Vt,
        work,
        &lwork,
        iwork,
        info
    );
}

void read_matrix (
    const char* filename,
    double* A,
    size_t* m,
    size_t* n,
    size_t* ld
)
{
    FILE* in;

    size_t i;
    size_t j;

    in = (FILE*)(NULL);

    i = 0U;
    j = 0U;

    if (!(filename && A && m && n && ld))
        return;

    in = fopen(filename, "rt");

    if (!in)
        return;

    if (!(fscanf(in, " %lu %lu", m, n) == 2U))
    {
        fclose(in);
        in = (FILE*)(NULL);

        return;
    }
    *ld = compute_ld(*m, *n);

    if (!(*m && *n))
    {
        fclose(in);
        in = (FILE*)(NULL);

        return;
    }

    for (i = 0U; i < *m; ++i)
        for (j = 0U; j < *n; ++j)
            if (!(fscanf(in, " %lf", A + index(*ld, i, j)) == 1U))
            {
                fclose(in);
                in = (FILE*)(NULL);

                return;
            }

    fclose(in);
    in = (FILE*)(NULL);
}

void print_matrix (
    const double* A,
    size_t m,
    size_t n,
    size_t ld
)
{
    size_t i;
    size_t j;

    i = 0U;
    j = 0U;

    if (!(A && m && n && ld))
        return;

    for (i = 0U; i < m; ++i)
    {
        printf("%.4f", *(A + i));
        for (j = 1U; j < n; ++j)
            printf("\t%.4f", *(A + j * ld + i));
        printf("\n");
    }
    printf("\n");
}

int main (int argc, char** argv)
{
    size_t m;
    size_t n;

    size_t ld_A;
    size_t ld_U;
    size_t ld_Vt;

    double A[LEN_MAX];

    double U[LEN_MAX];
    double Vt[LEN_MAX];
    double s[LEN_MAX];

    int lwork;

    double* work;
    /*int iwork[LEN_MAX];*/

    int info;

    m = 0U;
    n = 0U;

    ld_A = 0U;
    ld_U = 0U;
    ld_Vt = 0U;

    lwork = 0;

    work = (double*)(NULL);

    info = 0;

    memset(A, 0, (LEN_MAX) * sizeof *A);
    memset(U, 0, (LEN_MAX) * sizeof *U);
    memset(Vt, 0, (LEN_MAX) * sizeof *Vt);
    memset(s, 0, (LEN_MAX) * sizeof *s);

    /*memset(iwork, 0, (LEN_MAX) * sizeof *iwork);*/

    read_matrix("mat.txt", A, &m, &n, &ld_A);
    ld_U = compute_ld(m, m);
    ld_Vt = compute_ld(n, n);

    print_matrix(A, m, n, ld_A);

    lwork = -1;

    work = (double*)malloc(sizeof *work);

    if (!work)
        exit(EXIT_FAILURE);

    memset(work, 0, sizeof *work);

    dgesvd_wrapper(
        m,
        n,
        A,
        ld_A,
        U,
        ld_U,
        Vt,
        ld_Vt,
        s,
        all,
        all,
        lwork,
        work,
        &info
    );

    lwork = (int)(*work);

    free(work);
    work = (double*)(NULL);

    work = (double*)malloc((size_t)lwork * sizeof *work);

    if (!work)
        exit(EXIT_FAILURE);

    memset(work, 0, (size_t)lwork * sizeof *work);

    dgesvd_wrapper(
        m,
        n,
        A,
        ld_A,
        U,
        ld_U,
        Vt,
        ld_Vt,
        s,
        none,
        none,
        lwork,
        work,
        &info
    );

    printf("%d\n\n", info);

    print_matrix(U, m, m, ld_U);
    print_matrix(Vt, n, n, ld_Vt);
    print_matrix(s, 1U, m < n ? m : n, 1U);

    memset(work, 0, (size_t)lwork * sizeof *work);

    free(work);
    work = (double*)(NULL);

    return EXIT_SUCCESS;
}
