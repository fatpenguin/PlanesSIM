planes_status instantiate_planes(const size_t number, struct planes_t **planes) {
    // Calcul taille avec padding pour alignement
    size_t size = sizeof(struct planes_t);
    
    // BLOC HOT (contigu pour update_planes)
    size += number * sizeof(int);           // distance
    size += number * sizeof(int);           // speed
    size += number * sizeof(bool);          // used
    
    // Padding pour aligner booker_id (optionnel mais propre)
    size = (size + _Alignof(int8_t) - 1) & ~(_Alignof(int8_t) - 1);
    
    // BLOC COLD
    size += number * sizeof(int8_t);        // booker_id
    
    // Padding CRUCIAL pour aligner char** sur 8 bytes
    size = (size + _Alignof(char*) - 1) & ~(_Alignof(char*) - 1);
    
    size += number * sizeof(char *);        // names pointers
    size += number * NAME_MAX_LENGTH;       // names data

    // Allocation alignée sur cache line (64 bytes)
    struct planes_t *temp;
    int ret = posix_memalign((void**)&temp, 64, size);
    if (ret != 0 || temp == NULL) {
        PLANE_ERR_NUM = PLANE_ALLOCATION_FAILED;
        return ERROR;
    }
    memset(temp, 0, size);  // Équivalent de calloc
    
    // Placement des pointeurs
    char *p = (char *)(temp + 1);
    
    // BLOC HOT - collé pour update_planes()
    temp->distance = (int *)p;
    p += number * sizeof(int);
    
    temp->speed = (int *)p;
    p += number * sizeof(int);
    
    temp->used = (bool *)p;
    p += number * sizeof(bool);
    
    // Aligner pour booker_id
    p = (char *)(((uintptr_t)p + _Alignof(int8_t) - 1) & ~(_Alignof(int8_t) - 1));
    
    // BLOC COLD
    temp->booker_id = (int8_t *)p;
    p += number * sizeof(int8_t);
    
    // Aligner OBLIGATOIREMENT pour char** (8 bytes)
    p = (char *)(((uintptr_t)p + _Alignof(char*) - 1) & ~(_Alignof(char*) - 1));
    
    temp->names = (char **)p;
    p += number * sizeof(char *);
    
    // Données des noms
    char *names_data = p;
    for (size_t i = 0; i < number; i++) {
        temp->names[i] = names_data + i * NAME_MAX_LENGTH;
    }
    
    temp->nbr_planes = number;
    temp->available = number;
    
    *planes = temp;
    return SUCCESS;
}
