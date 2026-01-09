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


planes_status destroy_planes(struct planes_t **planes) {
    if (*planes == NULL) {
        PLANE_ERR_NUM = EMPTY_PLANES_POINTER;
        return ERROR;
    }
    free(*planes);
    *planes = NULL;
    return SUCCESS;
}
/// [END] MEMORY ALLOCATION CODE

/// BOOKING CODE
planes_status book_planes(const int8_t booker_id, const char *names[], size_t number, struct planes_t **planes) {
    struct planes_t *temp = *planes;
    if (number > temp->available) {
        PLANE_ERR_NUM = INVALID_NBR_BOOKING;
        return ERROR;
    }

    size_t index_start_search = 0;
    for (size_t i = 0; i < number; i++) {
        if (names[i] == NULL) {
            PLANE_ERR_NUM = INVALID_NAME_POINTER;
            return ERROR;
        }

        int found = 0;
        for (size_t j = index_start_search; j < temp->nbr_planes; j++) {
            if (!temp->used[j]) {
                strncpy(temp->names[j], names[i], NAME_MAX_LENGTH - 1);
                temp->names[j][NAME_MAX_LENGTH - 1] = '\0';
                index_start_search = j + 1;
                temp->used[j] = true;
                temp->booker_id[j] = booker_id;
                temp->available--;
                found = 1;
                break;
            }
        }

        if (!found) {
            PLANE_ERR_NUM = INVALID_NBR_BOOKING;
            return ERROR;
        }
    }

    return SUCCESS;
}
/// [END] BOOKING CODE
/// PLANE UPDATE
planes_status update_planes(struct planes_t *planes, int delta_t) {
    if (planes == NULL) {
        PLANE_ERR_NUM = EMPTY_PLANES_POINTER;
        return ERROR;
    }
    for (size_t i = 0; i < planes->nbr_planes; i++) {
        planes->distance[i] -= planes->speed[i] * delta_t & -(int)planes->used[i];
    }
    return SUCCESS;
}
