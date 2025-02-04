#include <singular/Singular/libsingular.h>
#include <iostream>

// Function to print an ideal
void id_Write(const ideal I, const ring r) {
    for (int i = 0; i < IDELEMS(I); i++) {
        std::cout << pString((poly)I->m[i]) << std::endl;
    }
}

int main() {
    // Initialize Singular
    siInit((char *)"/home/atraore/Singular4/lib/libSingular.so");

    // Construct the ring Z/32003[x1, x2, ..., x52]
    char **n = (char**)omalloc(52 * sizeof(char*));  // 52 variables
    for (int i = 0; i < 52; i++) {
        n[i] = omStrDup(("x" + std::to_string(i+1)).c_str());  // Generate variable names x1, x2, ..., x52
    }

    ring R = rDefault(32003, 52, n);
    rChangeCurrRing(R);  // Set the current ring to R

    // Create and initialize polynomials p1, p2, ..., p52 dynamically
    ideal L = idInit(52, 1);  // 52 generators for the ideal
    for (int i = 0; i < 52; i++) {
        poly p = p_ISet(1, R);  // Set polynomial with degree (1)
        pSetExp(p, i + 1, 1);  // Set exponent for the i+1-th variable
        L->m[i] = pCopy(p);  // Copy the polynomial into the ideal
    }

/*     // Print the ideal's generators
    std::cout << "Generators of ideal L: \n";
    id_Write(L, R); */

    // Compute the variety (ideal's associated variety)
    ideal variety_ideal_std = kStd(L, currRing->qideal, testHomog, NULL);
    std::cout << "Variety of the ideal: \n";
    id_Write(variety_ideal_std, R);

    int tt = IDELEMS(variety_ideal_std);
    std::cout << "Variety ideal size: " << tt << std::endl;

    // Clean up
    for (int i = 0; i < 52; i++) {
        pDelete(&L->m[i]);  // Clean up the polynomials in the ideal
    }
    idDelete(&L);  // Delete the ideal
    rKill(R);  // Clean up the ring

    // Finish up with Singular interpreter
    currentVoice = feInitStdin(NULL);
    int err = iiAllStart(NULL, "int ver=system(\"version\");\n", BT_proc, 0);
    if (err)
        errorreported = 0; // Reset error handling
    printf("Interpreter returns %d\n", err);

    return 0;
}
